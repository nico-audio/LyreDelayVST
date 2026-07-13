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

PresetPanel::PresetPanel(PresetManager& pm) : presetManager(pm), 
                                              previousPresetButton("", "<", LabeledButton::ButtonType::Text, LabeledButton::ButtonSize::Small),
                                              nextPresetButton("", ">", LabeledButton::ButtonType::Text, LabeledButton::ButtonSize::Small),
                                              saveButton("", "", LabeledButton::ButtonType::Image, LabeledButton::ButtonSize::Small),
                                              deleteButton("", "", LabeledButton::ButtonType::Image, LabeledButton::ButtonSize::Small)
{
    saveButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    deleteButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    
    // ADD LISTENER
    previousPresetButton.getButton().addListener(this);
    nextPresetButton.getButton().addListener(this);
    saveButton.getButton().addListener(this);
    deleteButton.getButton().addListener(this);
    presetList.addListener(this);
    
    // MAKE VISIBLE
    addAndMakeVisible(previousPresetButton);
    addAndMakeVisible(nextPresetButton);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(deleteButton);
    addAndMakeVisible(presetList);

    // TOGGLE STATE
    previousPresetButton.setClickingTogglesState(false);
    nextPresetButton.setClickingTogglesState(false);
    saveButton.setClickingTogglesState(false);
    deleteButton.setClickingTogglesState(false);
    
    // PRESET LIST
    presetList.setTextWhenNothingSelected("Lyre Init");
    loadPresetList();
    presetList.setLookAndFeel(&comboLF);

    // IMAGE BUTTONS
    auto saveIcon = juce::ImageCache::getFromMemory(BinaryData::save_button_4_png, BinaryData::save_button_4_pngSize);
    auto saveIconPressed = juce::ImageCache::getFromMemory(BinaryData::save_button_2_png, BinaryData::save_button_2_pngSize);
    saveButton.setImage(saveIcon, saveIcon, saveIconPressed);
    saveButton.setImageSize(30, 30);

    auto deleteIcon = juce::ImageCache::getFromMemory(BinaryData::deletebutton_2_png, BinaryData::deletebutton_2_pngSize);
    auto deleteIconDown = juce::ImageCache::getFromMemory(BinaryData::deletebutton_1_png, BinaryData::deletebutton_1_pngSize);
    deleteButton.setImage(deleteIcon, deleteIcon, deleteIcon);
    deleteButton.setImageSize(30, 30);

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
    //const auto container = getLocalBounds().reduced(6);
    //auto bounds = container;

    previousPresetButton.setTopLeftPosition(2, 9);
    presetList.setBounds(previousPresetButton.getRight() + 5, 30, 120, 25 );
    nextPresetButton.setTopLeftPosition(presetList.getRight() + 5, previousPresetButton.getY());
    saveButton.setTopLeftPosition(nextPresetButton.getRight(), previousPresetButton.getY() + 5);
    deleteButton.setTopLeftPosition(saveButton.getRight(), previousPresetButton.getY() + 5);
}

//==============================================================================

void PresetPanel::buttonClicked(juce::Button* button)
{
    if (button == &previousPresetButton.getButton()) {
        const auto index = presetManager.loadPreviousPreset();
        presetList.setSelectedItemIndex(index, juce::dontSendNotification);
    }

    if (button == &nextPresetButton.getButton()) {
        const auto index = presetManager.loadNextPreset();
        presetList.setSelectedItemIndex(index, juce::dontSendNotification);
    }

    if (button == &saveButton.getButton()) {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Please enter the name of the preset to save",
            PresetManager::defaultDirectory, "*." + PresetManager::extension
    );

        fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser& chooser) {
            const auto resultFile = chooser.getResult();
            presetManager.savePreset(resultFile.getFileNameWithoutExtension());
            loadPresetList();
        });
    }

    if (button == &deleteButton.getButton()) {
        presetManager.deletePreset(presetManager.getCurrentPreset());
        loadPresetList();
        // add prompt warning!
    }
}

void PresetPanel::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &presetList) {
        presetManager.loadPreset(presetList.getItemText(presetList.getSelectedItemIndex()));
    }
}

void PresetPanel::loadPresetList()
{
    presetList.clear(juce::dontSendNotification);
    const auto allPresets = presetManager.getAllPresets();
    const auto currentPreset = presetManager.getCurrentPreset();
    presetList.addItemList(allPresets, 1);
    presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);
}