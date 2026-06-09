/*
  ==============================================================================

    PresetPanel.cpp
    Created: 3 Jun 2026 3:55:01pm
    Author:  Nico V.

  ==============================================================================
*/

#include "PresetPanel.h"

//=============================================================================
// CONSTRUCTOR/ DESTRUCTOR
//=============================================================================

PresetPanel::PresetPanel(juce::AudioProcessorValueTreeState& apvts) : previousPresetButton("", "<", LabeledButton::ButtonType::Text, LabeledButton::ButtonSize::Small),
                                                                      nextPresetButton("", ">", LabeledButton::ButtonType::Text, LabeledButton::ButtonSize::Small),
                                                                      saveButton("", "S", LabeledButton::ButtonType::Text, LabeledButton::ButtonSize::Small),
                                                                      deleteButton("", "D", LabeledButton::ButtonType::Text, LabeledButton::ButtonSize::Small)
{
    saveButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    deleteButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    
    previousPresetButton.getButton().addListener(this);
    nextPresetButton.getButton().addListener(this);
    saveButton.getButton().addListener(this);
    deleteButton.getButton().addListener(this);
    presetList.addListener(this);
    
    addAndMakeVisible(previousPresetButton);
    addAndMakeVisible(nextPresetButton);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(deleteButton);
    addAndMakeVisible(presetList);

    previousPresetButton.setClickingTogglesState(false);
    nextPresetButton.setClickingTogglesState(false);
    saveButton.setClickingTogglesState(false);
    deleteButton.setClickingTogglesState(false);

    presetList.setLookAndFeel(&comboLF);
}

PresetPanel::~PresetPanel()
{
    previousPresetButton.getButton().removeListener(this);
    nextPresetButton.getButton().removeListener(this);
    saveButton.getButton().removeListener(this);
    deleteButton.getButton().removeListener(this);
    presetList.removeListener(this);

    presetList.setLookAndFeel(nullptr);
}

//==============================================================================
// LAYOUT
//==============================================================================

void PresetPanel::resized()
{
    const auto container = getLocalBounds().reduced(6);
    auto bounds = container;

    previousPresetButton.setBounds(0, 11, 60, 60);
    presetList.setTextWhenNothingSelected("Lyre Init");
    presetList.setBounds(previousPresetButton.getRight(), 30, 120, 25 );
    nextPresetButton.setBounds(180, 11, 60, 60);
    saveButton.setBounds(210, 11, 60, 60);
    deleteButton.setBounds(240, 11, 60, 60);
   
}

//==============================================================================

void PresetPanel::buttonClicked(juce::Button*)
{
    /*
    if (button == &previousPresetButton){
        //  go to previous preset
    }
    else if (button == &nextPresetButton) {
        // go to next preset
    }
     */
}

void PresetPanel::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{

}