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
    juce::AudioParameterFloat* gainParam;
};