/*
  ==============================================================================

    GranularEngine.cpp
    Created: 30 Mar 2026 9:50:16am
    Author:  Nico V.

  ==============================================================================
*/

#include <cmath>
#include "GranularEngine.h"

void GranularEngine::prepare(double sr)
{
    jassert(sr > 0.0);

    sampleRate = sr;
    normSmoothingCoefficient = 1.0f - std::exp(-1.0f / (0.01f * float(sampleRate)));

    reset();
}

void GranularEngine::reset()
{
    for (auto& grain : grainPool)
    {
        grain.reset();
    }

    samplesUntilNextGrain = 0;
    smoothedNorm = 1.0f;
}

//==============================================================================
// PARAMETERS
//==============================================================================

void GranularEngine::setParameters(float gDensity, float gSizeMs, float pitchSt, float tex, bool gState)
{
    if (gState && !isActive) {
        samplesUntilNextGrain = 0;
    }
    
    density = gDensity;
    grainSizeMs = gSizeMs;
    pitchSemitones = pitchSt;
    texture = juce::jlimit(0.0f, 1.0f, tex);
    isActive = gState;

    // Granular scheduler counter
    if (density > 0.0f)
    {
        samplesBetweenGrains = static_cast<int>(sampleRate / density);
        samplesBetweenGrains = std::max(1, samplesBetweenGrains);
    }
    else
    {
        samplesBetweenGrains = -1;
    }
}

//==============================================================================
// GRAIN POOL
//==============================================================================

Grain* GranularEngine::findAvailableGrain(std::array<Grain, maxGrains>& pool)
{
    for (auto& grain : pool)
    {
        if (!grain.isActive)
            return &grain;
    }
    return nullptr;
}

void GranularEngine::spawnGrain(Grain& grain, int startIndex, int bufferSize, int grainDurationSamples, float pitchRatio)
{
    juce::ignoreUnused(bufferSize);
    
    // Safety
    jassert(bufferSize > 0);
    jassert(grainDurationSamples > 0);
    jassert(startIndex >= 0 && startIndex < bufferSize);

    grain.isActive = true;
    grain.samplesPlayed = 0;
    grain.grainDuration = grainDurationSamples;
    grain.startIndex = static_cast<float>(startIndex);

    grain.stepSize = pitchRatio;
    grain.grainIndexPosition = grain.startIndex;
}

//==============================================================================
// PROCESS
//==============================================================================

void GranularEngine::processGrain(Grain& grain, DelayLine& delayLineL, DelayLine& delayLineR, float& outL, float& outR)
{
    outL = 0.0f;
    outR = 0.0f;
    
    if (!grain.isActive) {
        return;
    }

    int bufferSize = delayLineL.getBufferLength();

    //==============================================================================
    // LINEAR INTERPOLATION
    //==============================================================================
    
    float exactPosition = grain.grainIndexPosition;
    int indexA = static_cast<int>(exactPosition);
    float fraction = exactPosition - static_cast<float>(indexA);
    
    int indexB = indexA + 1;
    
    if (indexB >= bufferSize) {
        indexB -= bufferSize;
    }

    jassert(indexA >= 0 && indexA < bufferSize);
    jassert(indexB >= 0 && indexB < bufferSize);

    float sampleL_A = delayLineL.readAtIndex(indexA);
    float sampleL_B = delayLineL.readAtIndex(indexB);
    float sampleL = sampleL_A + fraction * (sampleL_B - sampleL_A);

    float sampleR_A = delayLineR.readAtIndex(indexA);
    float sampleR_B = delayLineR.readAtIndex(indexB);
    float sampleR = sampleR_A + fraction * (sampleR_B - sampleR_A);

    //==============================================================================
    // WINDOWING
    //==============================================================================

    float window;
    if (grain.grainDuration <= 1) {
        window = 1.0f;
    }
    else
    {
        float phase = static_cast<float>(grain.samplesPlayed) / static_cast<float>(grain.grainDuration - 1);
        window = 0.5f * (1.0f - std::cos(juce::MathConstants<float>::twoPi * phase));
    }

    outL = sampleL * window;
    outR = sampleR * window;

    //==============================================================================
    // POSITION UPDATE
    //==============================================================================

    grain.grainIndexPosition += grain.stepSize;

    if (grain.grainIndexPosition >= static_cast<float>(bufferSize)) {
        grain.grainIndexPosition -= static_cast<float>(bufferSize);
    }
    else if (grain.grainIndexPosition < 0.0f) {
        grain.grainIndexPosition += static_cast<float>(bufferSize);
    }
    
    grain.samplesPlayed++;

    if (grain.samplesPlayed >= grain.grainDuration) {
        grain.isActive = false;
    }
}

void GranularEngine::process(float& grainSumL, float& grainSumR, DelayLine& delayL, DelayLine& delayR)
{
    if (!isActive || samplesBetweenGrains <= 0) {
        return;
    }

    float grainSizeSamples = msToSamples(grainSizeMs);
    float pitchRatio = std::pow(2.0f, pitchSemitones / 12.0f);

    samplesUntilNextGrain--;

    if (samplesUntilNextGrain <= 0)
    {
        Grain* availableGrain = findAvailableGrain(grainPool);
        
        // Texture - density jitter
        int jitteredInterval = samplesBetweenGrains;

        if (availableGrain != nullptr){
            if (texture > 0.0f) {
                const float randomSigned = textureRange.nextFloat() * 2.0f - 1.0f;
                const float maxDensityJitter = 0.5f;
                const float densityJitterAmount = texture * maxDensityJitter;

                const float factor = 1.0f + randomSigned * densityJitterAmount;
                jitteredInterval = juce::jmax(1, static_cast<int>(std::round(samplesBetweenGrains * factor)));
            }

            // Texture - grain size jitter
            int jitteredGrainSizeSamples = static_cast<int>(std::round(grainSizeSamples));

            if (texture > 0.0f) {
                const float randomSigned2 = textureRange.nextFloat() * 2.0f - 1.0f;
                const float maxGrainJitter = 1.0f;
                const float grainJitterAmount = texture * maxGrainJitter;

                const float factor2 = 1.0f + randomSigned2 * grainJitterAmount;

                jitteredGrainSizeSamples = juce::jmax(1, static_cast<int>(std::round(grainSizeSamples * factor2)));
            }

            // Texture - position jitter
            int positionJitterSamples = 0;

            if (texture > 0.0f)
            {
                const float random = textureRange.nextFloat() * 2.0f - 1.0f;
                const float maxPositionJitter = 0.5f;
                const float jitterAmount = texture * maxPositionJitter;

                const float factor = random * jitterAmount;

                positionJitterSamples = static_cast<int>(std::round(jitteredGrainSizeSamples * factor));
            }

            int startIndex = delayL.getWriteIndex() - jitteredGrainSizeSamples + positionJitterSamples;
            const int bufferSize = delayL.getBufferLength();
            startIndex = (startIndex % bufferSize + bufferSize) % bufferSize;

            spawnGrain(*availableGrain, startIndex, bufferSize, jitteredGrainSizeSamples, pitchRatio);

            //DBG("spawn grain!");
        }
        samplesUntilNextGrain = jitteredInterval;
    }

    
    //==============================================================================
    // OUTPUT
    //==============================================================================
    
    // Sum grains    
    grainSumL = 0.0f;
    grainSumR = 0.0f;
    
    int activeGrains = 0;

    for (auto& grain : grainPool)
    {
        if (grain.isActive) {
            float outL, outR;
            processGrain(grain, delayL, delayR, outL, outR);
            grainSumL += outL;
            grainSumR += outR;
            activeGrains++;
        }
    }

    // Normalize (with one-pole smoothing)
    if (activeGrains > 0) {
        float targetNorm = 1.0f / static_cast<float>(activeGrains);
        smoothedNorm += (targetNorm - smoothedNorm) * normSmoothingCoefficient;
        grainSumL *= smoothedNorm;
        grainSumR *= smoothedNorm;
    }
}