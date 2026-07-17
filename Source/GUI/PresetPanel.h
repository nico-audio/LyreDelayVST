/*
  ==============================================================================

    PresetPanel.h
    Created: 3 Jun 2026 3:55:01pm
    Author:  Nico V.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "LabeledButton.h"
#include "../DSP/ParameterHelper.h"
#include "LookAndFeel.h"
#include "../Model/PresetManager.h"

class PresetPanel : public juce::Component, public juce::Button::Listener, public juce::ComboBox::Listener
{
public:
    PresetPanel(PresetManager& pm, GDelayAudioProcessor& processor);
    ~PresetPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button*) override;
    void comboBoxChanged(juce::ComboBox* comboBoxChanged) override;
    void loadPresetList();

private:
    PresetManager& presetManager;
    GDelayAudioProcessor& audioProcessor;
    ComboBoxLookAndFeel comboLF;

    LabeledButton previousPresetButton;
    LabeledButton nextPresetButton;
    LabeledButton saveButton; 
    LabeledButton deleteButton;
    LabeledButton bypassButton;
    juce::ComboBox presetList;
    std::unique_ptr<juce::FileChooser> fileChooser;

    juce::Rectangle<int> logoBounds;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
};
