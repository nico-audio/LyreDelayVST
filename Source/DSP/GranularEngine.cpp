/*
  ==============================================================================

    GranularEngine.cpp
    Created: 30 Mar 2026 9:50:16am
    Author:  Nico V.

  ==============================================================================
*/

#include <cmath>
#include "GranularEngine.h"

void GranularEngine::prepare(double sr, int maxDelaySamples)
{
    jassert(sr > 0.0);
    jassert(maxDelaySamples > 0);

    sampleRate = sr;

    for (auto& grain : grainPool)
    {
        grain.reset();
    }

    samplesUntilNextGrain = 0;
}

void GranularEngine::reset()
{
    for (auto& grain : grainPool)
    {
        grain.reset();
    }

    samplesUntilNextGrain = 0;
}

void GranularEngine::setParameters(float gDensity, float gSizeMs, float pitchSt, float tex, bool gState)
{
    density = gDensity;
    grainSizeMs = gSizeMs;
    pitchSemitones = pitchSt;
    texture = juce::jlimit(0.0f, 1.0f, tex);
    isActive = gState;

    // Granular scheduler counter
    if (density > 0.0f)
    {
        samplesBetweenGrains = (int)(sampleRate / density);
        samplesBetweenGrains = std::max(1, samplesBetweenGrains);
    }
    else
    {
        samplesBetweenGrains = -1;
    }
}

Grain* GranularEngine::findAvailableGrain(std::array<Grain, maxGrains>& pool)
{
    for (auto& grain : pool)
    {
        if (!grain.isActive)
            return &grain;
    }
    return nullptr;
}

void GranularEngine::spawnGrain(Grain& grain, int delayWriteIndex, int bufferSize, int grainDurationSamples, float pitchRatio)
{
    // Safety
    jassert(bufferSize > 0);
    jassert(grainDurationSamples > 0);
    jassert(grain.startIndex >= 0 && grain.startIndex < bufferSize);

    grain.isActive = true;
    grain.samplesPlayed = 0;
    grain.grainDuration = grainDurationSamples;

    grain.startIndex = delayWriteIndex - grainDurationSamples;

    grain.stepSize = pitchRatio;

    // wrapping
    if (grain.startIndex < 0)
        grain.startIndex += bufferSize;

    grain.grainIndexPosition = grain.startIndex;
}

float GranularEngine::processGrain(Grain& grain, DelayLine& delayLine)
{
    if (!grain.isActive) {
        return 0.0f;
    }

    int bufferSize = delayLine.getBufferLength();
    int readIndex = static_cast<int>(grain.grainIndexPosition);

    if (readIndex < 0) {
        readIndex += bufferSize;
    }

    else if (readIndex >= bufferSize) {
        readIndex -= bufferSize;
    }

    // Safety: after wrapping, readIndex must be valid
    jassert(readIndex >= 0 && readIndex < bufferSize);

    float sample = delayLine.readAtIndex(readIndex);

    float phase = static_cast<float>(grain.samplesPlayed) /
        static_cast<float>(grain.grainDuration - 1);

    float window = 0.5f * (1.0f - std::cos(juce::MathConstants<float>::twoPi * phase));
    window *= juce::Decibels::decibelsToGain(2.0f); // Normalize Hann to unity RMS
    sample *= window;

    //int windowSize = 1024;
    //juce::dsp::WindowingFunction<float> window(windowSize, juce::dsp::WindowingFunction<float>::hann);

    grain.grainIndexPosition += grain.stepSize;
    grain.samplesPlayed++;

    if (grain.samplesPlayed >= grain.grainDuration) {
        grain.isActive = false;
    }

    return sample;
}

void GranularEngine::process(float& grainSumL, float& grainSumR, DelayLine& delayL, DelayLine& delayR)
{
    float grainSizeSamples = msToSamples(grainSizeMs);
    float pitchRatio = std::pow(2.0f, pitchSemitones / 12.0f);

    if (!isActive || samplesBetweenGrains <= 0) {
        return;
    }
    
    samplesUntilNextGrain--;

    if (samplesUntilNextGrain <= 0)
    {
        Grain* availableGrain = findAvailableGrain(grainPool);

        if (availableGrain != nullptr){
            // Texture - density jitter
            int jitteredInterval = samplesBetweenGrains;

            if (texture > 0.0f) {
                const float randomSigned = textureRange.nextFloat() * 2.0f - 1.0f;
                const float maxDensityJitter = 0.5f;
                const float densityJitterAmount = texture * maxDensityJitter;

                const float factor = 1.0f + randomSigned * densityJitterAmount;
                jitteredInterval = juce::jmax(1, (int)std::round(samplesBetweenGrains * factor));

                DBG("Texture density jitter: base=" << samplesBetweenGrains
                    << " factor=" << factor
                    << " jittered=" << jitteredInterval);
            }
            // Texture - grain size jitter
            int jitteredGrainSizeSamples = grainSizeSamples;

            if (texture > 0.0f) {
                const float randomSigned2 = textureRange.nextFloat() * 2.0f - 1.0f;
                const float maxGrainJitter = 1.0f;
                const float grainJitterAmount = texture * maxGrainJitter;

                const float factor2 = 1.0f + randomSigned2 * grainJitterAmount;

                jitteredGrainSizeSamples = juce::jmax(1, (int)std::round(grainSizeSamples * factor2));

                DBG("Texture grain size jitter: base=" << grainSizeSamples
                    << " factor=" << factor2
                    << " jittered=" << jitteredGrainSizeSamples);
            }

            // Texture - position jitter
            int positionJitterSamples = 0;

            if (texture > 0.0f)
            {
                const float random = textureRange.nextFloat() * 2.0f - 1.0f;
                const float maxPositionJitter = 0.5f;
                const float jitterAmount = texture * maxPositionJitter;

                const float factor = random * jitterAmount;

                positionJitterSamples = (int)std::round(jitteredGrainSizeSamples * factor);

                DBG("Texture position jitter: " << positionJitterSamples << " samples");
            }

            int startIndex = delayL.getWriteIndex() - jitteredGrainSizeSamples + positionJitterSamples;
            const int bufferSize = delayL.getBufferLength();
            startIndex = (startIndex % bufferSize + bufferSize) % bufferSize;

            spawnGrain(*availableGrain, startIndex, bufferSize, jitteredGrainSizeSamples, pitchRatio);

            //spawnGrain(*availableGrain, delayLineL.getWriteIndex(), delayLineL.getBufferLength(), jitteredGrainSizeSamples, pitchRatio);
            DBG("spawn grain!");
        }
    }

    // Sum grains    
    grainSumL = 0.0f;
    grainSumR = 0.0f;
    
    int activeGrains = 0;
    for (auto& grain : grainPool)
    {
        if (grain.isActive) {
            grainSumL += processGrain(grain, delayL);
            grainSumR += processGrain(grain, delayR);
            activeGrains++;
        }
    }

    // Normalize
    if (activeGrains > 0) {
        float norm = 1.0f / activeGrains;
        grainSumL *= norm;
        grainSumR *= norm;
    }
}