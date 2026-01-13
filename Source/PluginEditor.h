/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"
#include "LookAndFeel.h"
#include "LevelMeter.h"
#include <melatonin_inspector/melatonin_inspector.h>


class GDelayAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                    private juce::AudioProcessorParameter::Listener
{
public:
    GDelayAudioProcessorEditor (GDelayAudioProcessor&);
    ~GDelayAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void parameterValueChanged(int, float) override;
    void parameterGestureChanged(int, bool) override {}

    void updateDelayKnobs(bool tempoSyncActive);

    GDelayAudioProcessor& audioProcessor;

    RotaryKnob gainKnob { "Gain", audioProcessor.apvts, gainParamID, true };
    RotaryKnob mixKnob{ "Mix", audioProcessor.apvts, mixParamID };
    RotaryKnob delayTimeKnob{ "Time", audioProcessor.apvts, delayTimeParamID };
    RotaryKnob feedbackKnob{ "Feedback", audioProcessor.apvts, feedbackParamID, true };
    RotaryKnob stereoKnob{ "Stereo", audioProcessor.apvts, stereoParamID, true, { 0, 0, 68, 76 } };
    RotaryKnob lowCutKnob{ "Low Cut", audioProcessor.apvts, lowCutParamID, true, { 0, 0, 68, 76 } };
    RotaryKnob highCutKnob{ "High Cut", audioProcessor.apvts, highCutParamID, true, { 0, 0, 68, 76 } };
    RotaryKnob delayNoteKnob{ "Note", audioProcessor.apvts, delayNoteParamID };
    
    juce::TextButton tempoSyncButton;

    juce::AudioProcessorValueTreeState::ButtonAttachment tempoSyncAttachment{ 
          audioProcessor.apvts, tempoSyncParamID.getParamID(), tempoSyncButton
    };

    juce::ImageButton bypassButton;

    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment{ 
        audioProcessor.apvts, bypassParamID.getParamID(), bypassButton
    };

    juce::TextButton granularToggleButton;

    juce::AudioProcessorValueTreeState::ButtonAttachment granularToggleAttachment{
        audioProcessor.apvts, granularToggleParamID.getParamID(), granularToggleButton
    };

    juce::GroupComponent grainGroup, delayGroup, outputGroup;

    MainLookAndFeel mainLF;

    LevelMeter meter;

    std::unique_ptr<melatonin::Inspector> inspector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GDelayAudioProcessorEditor)
};
