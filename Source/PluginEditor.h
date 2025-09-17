/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"

//==============================================================================
/**
*/
class GDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GDelayAudioProcessorEditor (GDelayAudioProcessor&);
    ~GDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    GDelayAudioProcessor& audioProcessor;

    juce::Slider slider;
    juce::Label label;

    juce::AudioProcessorValueTreeState::SliderAttachment attachment{
        audioProcessor.apvts, gainParamID.getParamID(), slider
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GDelayAudioProcessorEditor)
};
