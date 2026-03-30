/*
  ==============================================================================

    GranularEngine.h
    Created: 30 Mar 2026 9:50:16am
    Author:  Nico V.

  ==============================================================================
*/

#pragma once
#include <array>
#include <JuceHeader.h>
#include "Grain.h"
#include "DelayLine.h"

class GranularEngine
{
public:
    GranularEngine() = default;

    void prepare(double sampleRate, int maxDelaySamples);
    void reset();
    
    void setParameters(float gDensity, float gSizeMs, float pitchSt, float tex, bool gState);
    void process(float& grainSumL, float& grainSumR, DelayLine& delayL, DelayLine& delayR);

private:
    void spawnGrain(Grain& grain, int delayWriteIndex, int bufferSize, int grainDurationSamples, float pitchRatio);
    float processGrain(Grain& grain, DelayLine& delayLine);

    inline float msToSamples(float ms) const
    {
        return ms * 0.001f * sampleRate;
    }

    // sample rate is overriden later
    double sampleRate = 44100.0;

    // Parameters
    float density = 0.0f;
    float grainSizeMs = 0.0f;
    float pitchSemitones = 0.0f;
    float texture = 0.0f;
    bool isActive = false;

    // Scheduler
    int samplesBetweenGrains = 0;
    int samplesUntilNextGrain = 0;

    // Grain pool
    static constexpr int maxGrains = 60;
    std::array<Grain, maxGrains> grainPool;
    Grain* findAvailableGrain(std::array<Grain, maxGrains>& pool);

    // Random generator
    juce::Random textureRange;
};