/*
  ==============================================================================

    Parameters.h
    Created: 9 Sep 2025 12:38:54pm
    Author:  Nico V.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

const juce::ParameterID gainParamID{ "gain", 1 };
const juce::ParameterID delayTimeParamID{ "delayTime", 1 };

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

    static constexpr float minDelayTime { 5.0f };
    static constexpr float maxDelayTime { 5000.0f };
    static constexpr float delayTimeStepSize { 0.001f };
    static constexpr float delayTimeSkew { 0.25f };
    
private:    
    juce::AudioParameterFloat* gainParam;
    juce::AudioParameterFloat* delayTimeParam;
    juce::LinearSmoothedValue<float> gainSmoother;

    float targetDelayTime = 0.0f;
    float filterCoefficient = 0.0f;
};