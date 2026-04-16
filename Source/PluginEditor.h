/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DSP/ParameterHelper.h"
#include "GUI/RotaryKnob.h"
#include "GUI/LookAndFeel.h"
#include "GUI/LevelMeter.h"
#include "GUI/LabeledButton.h"
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
    void updateButtonEnabled(bool granularEnabled);

    GDelayAudioProcessor& audioProcessor;

    RotaryKnob gainKnob { "Gain", audioProcessor.apvts, Params::gainParamID, true };
    RotaryKnob mixKnob{ "Mix", audioProcessor.apvts, Params::mixParamID };
    RotaryKnob delayTimeKnob{ "Time", audioProcessor.apvts, Params::delayTimeParamID };
    RotaryKnob feedbackKnob{ "Feedback", audioProcessor.apvts, Params::feedbackParamID, true };
    RotaryKnob stereoKnob{ "Stereo", audioProcessor.apvts, Params::stereoParamID, true, RotaryKnob::KnobSize::Small };
    RotaryKnob lowCutKnob{ "Low Cut", audioProcessor.apvts, Params::lowCutParamID, true, RotaryKnob::KnobSize::Small };
    RotaryKnob highCutKnob{ "High Cut", audioProcessor.apvts, Params::highCutParamID, true, RotaryKnob::KnobSize::Small };
    RotaryKnob delayNoteKnob{ "Note", audioProcessor.apvts, Params::delayNoteParamID };
    RotaryKnob grainSize{ "Size", audioProcessor.apvts, Params::grainSizeParamID };
    RotaryKnob grainPitch{ "pitch", audioProcessor.apvts, Params::grainPitchParamID };
    RotaryKnob grainDensity{ "Density", audioProcessor.apvts, Params::grainDensityParamID };
    RotaryKnob textureKnob{ "Texture", audioProcessor.apvts, Params::textureParamID };


    LabeledButton tempoSyncButton{ "Sync", "", audioProcessor.apvts, Params::tempoSyncParamID, LabeledButton::ButtonSize::Medium };
    LabeledButton granularToggleButton{ "granular", "", audioProcessor.apvts, Params::granularToggleParamID, LabeledButton::ButtonSize::Large };
    
    juce::ImageButton bypassButton;
    juce::ImageButton randomizerButton;

    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment{
        audioProcessor.apvts, Params::bypassParamID.getParamID(), bypassButton
    };

    juce::AudioProcessorValueTreeState::ButtonAttachment randomizerAttachment{
        audioProcessor.apvts, Params::randomPushParamID.getParamID(), randomizerButton
    };

    juce::GroupComponent grainGroup, delayGroup, outputGroup;

    MainLookAndFeel mainLF;
    ButtonLookAndFeel buttonLF;

    LevelMeter meter;

    std::unique_ptr<melatonin::Inspector> inspector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GDelayAudioProcessorEditor)
};
