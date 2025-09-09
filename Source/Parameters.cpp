/*
  ==============================================================================

    Parameters.cpp
    Created: 9 Sep 2025 12:38:54pm
    Author:  Nico V.

  ==============================================================================
*/

#include "Parameters.h"

// Constructor
Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    auto* param = apvts.getParameter(gainParamID.getParamID());
    gainParam = dynamic_cast<juce::AudioParameterFloat*>(param);
}

// Plugin parameters
juce::AudioProcessorValueTreeState::ParameterLayout
Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        gainParamID,
        "Output gain",
        juce::NormalisableRange<float> { -12.0f, 12.0f },
        0.0f));

    return layout;
}
