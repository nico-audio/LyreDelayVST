/*
  ==============================================================================

    PresetPanel.h
    Created: 3 Jun 2026 3:55:01pm
    Author:  Nico V.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LabeledButton.h"
#include "../DSP/ParameterHelper.h"
#include "LookAndFeel.h"

class PresetPanel : public juce::Component, public juce::Button::Listener, public juce::ComboBox::Listener
{
public:
    PresetPanel(juce::AudioProcessorValueTreeState& apvts);
    ~PresetPanel() override;

    void resized() override;
    void buttonClicked(juce::Button*) override;
    void comboBoxChanged(juce::ComboBox* comboBoxChanged) override;

private:
    ComboBoxLookAndFeel comboLF;

    LabeledButton previousPresetButton;
    LabeledButton nextPresetButton;
    LabeledButton saveButton; 
    LabeledButton deleteButton;
    juce::ComboBox presetList;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
};
