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

    static constexpr float minDelayTime = 5.0f;
    static constexpr float maxDelayTime = 5000.0f;
    
private:    
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;
};