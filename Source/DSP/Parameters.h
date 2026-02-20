/*
  ==============================================================================

    Parameters.h
    Created: 9 Sep 2025 12:38:54pm
    Author:  Nico V.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ParameterHelper.h"

class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void update() noexcept;
    void smoothen() noexcept;

    float gain = 0.0f;
    float delayTime = 0.0f;
    float mix = 1.0f;
    float feedback = 0.0f;
    float panL = 0.0f;
    float panR = 1.0f;
    float lowCut = 20.0f;
    float highCut = 20000.0f;
    float grainSize = 1.0f;
    float pitch = 0.0f;
    float density = 0.0f;
    int delayNote = 0;
    bool tempoSync = false;
    bool bypassed = false;
    bool granularisActive = false;
   
    static constexpr float minDelayTime { 5.0f };
    static constexpr float maxDelayTime { 5000.0f };
    static constexpr float delayTimeStepSize { 0.001f };
    static constexpr float delayTimeSkew { 0.25f };

    static constexpr float minMix { 0.0f };
    static constexpr float maxMix { 100.0f };
    static constexpr float mixStepSize { 1.0f };
    static constexpr float defaultMix { 100.0f };

    static constexpr float minFilterCutoff { 20.0f };
    static constexpr float maxFilterCutoff { 20000.0f };
    static constexpr float filterStepSize { 1.0f };
    static constexpr float filterSkewFactor { 0.3f };
    static constexpr float defaultHighCutoff { 20000.0f };
    static constexpr float defaultLowCutoff { 20.0f };
    
    static constexpr float minGrainSize { 1.0f };
    static constexpr float maxGrainSize { 100.0f };
    static constexpr float grainStepSize { 1.0f };
    static constexpr float defaultSize { 1.0f };

    static constexpr float minPitch { -24.0f };
    static constexpr float maxPitch { 24.0f };
    static constexpr float pitchStepSize { 0.01f };
    static constexpr float defaultPitch { 0.0f };

    static constexpr float minDensity { 0.0f };
    static constexpr float maxDensity { 100.0f };
    static constexpr float densityStepSize { 1.0f };
    static constexpr float defaultDensity { 0.0f };

    juce::AudioParameterBool* tempoSyncParam;
    juce::AudioParameterBool* bypassParam;
    juce::AudioParameterBool* granularToggleParam;
    
private:
    // Gain
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;

    // Delay line
    juce::AudioParameterFloat* delayTimeParam;
    float targetDelayTime = 0.0f;
    float filterCoefficient = 0.0f;

    // Mix
    juce::AudioParameterFloat* mixParam;
    juce::LinearSmoothedValue<float> mixSmoother;

    // Feedback
    juce::AudioParameterFloat* feedbackParam;
    juce::LinearSmoothedValue<float> feedbackSmoother;

    // Stereo Width
    juce::AudioParameterFloat* stereoParam;
    juce::LinearSmoothedValue<float> stereoSmoother;

    // SVF Filter - high and low cut
    juce::AudioParameterFloat* lowCutParam;
    juce::LinearSmoothedValue<float> lowCutSmoother;

    juce::AudioParameterFloat* highCutParam;
    juce::LinearSmoothedValue<float> highCutSmoother;

    // Tempo sync
    juce::AudioParameterChoice* delayNoteParam;

    // Grain size
    juce::AudioParameterFloat* sizeParam;
    juce::LinearSmoothedValue<float> sizeSmoother;

    // Grain pitch
    juce::AudioParameterFloat* pitchParam;
    juce::LinearSmoothedValue<float> pitchSmoother;

    // Grain density
    juce::AudioParameterFloat* densityParam;
    juce::LinearSmoothedValue<float> densitySmoother;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)
};