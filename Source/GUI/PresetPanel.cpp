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

PresetPanel::PresetPanel(PresetManager& pm, GDelayAudioProcessor& processor) : presetManager(pm),
                                              audioProcessor(processor),
                                              previousPresetButton("", "<", LabeledButton::ButtonType::Text, LabeledButton::ButtonSize::Small),
                                              nextPresetButton("", ">", LabeledButton::ButtonType::Text, LabeledButton::ButtonSize::Small),
                                              saveButton("", "", LabeledButton::ButtonType::Image, LabeledButton::ButtonSize::Small),
                                              deleteButton("", "", LabeledButton::ButtonType::Image, LabeledButton::ButtonSize::Small),
                                              bypassButton("", "", audioProcessor.apvts, Params::bypassParamID, LabeledButton::ButtonType::Image, LabeledButton::ButtonSize::Small)
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
    bypassButton.getButton().addListener(this);
    
    // MAKE VISIBLE
    addAndMakeVisible(previousPresetButton);
    addAndMakeVisible(nextPresetButton);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(deleteButton);
    addAndMakeVisible(presetList);
    addAndMakeVisible(bypassButton);

    // TOGGLE STATE
    previousPresetButton.setClickingTogglesState(false);
    nextPresetButton.setClickingTogglesState(false);
    saveButton.setClickingTogglesState(false);
    deleteButton.setClickingTogglesState(false);
    bypassButton.setClickingTogglesState(true);
    
    // PRESET LIST
    presetList.setTextWhenNothingSelected("Lyre Init");
    loadPresetList();
    presetList.setLookAndFeel(&comboLF);

    // IMAGE BUTTONS
    auto saveIcon = juce::ImageCache::getFromMemory(BinaryData::save_button_4_png, BinaryData::save_button_4_pngSize);
    auto saveIconPressed = juce::ImageCache::getFromMemory(BinaryData::save_button_2_png, BinaryData::save_button_2_pngSize);
    saveButton.setImage(saveIcon, saveIconPressed, saveIconPressed, juce::Colours::white.withAlpha(0.1f), juce::Colours::transparentWhite);
    saveButton.setImageSize(25, 25);

    auto deleteIcon = juce::ImageCache::getFromMemory(BinaryData::deletebutton_2_png, BinaryData::deletebutton_2_pngSize);
    auto deleteIconDown = juce::ImageCache::getFromMemory(BinaryData::deletebutton_1_png, BinaryData::deletebutton_1_pngSize);
    deleteButton.setImage(deleteIcon, deleteIcon, deleteIcon, juce::Colours::white.withAlpha(0.1f), juce::Colours::transparentWhite);
    deleteButton.setImageSize(25, 25);

    auto bypassIcon = juce::ImageCache::getFromMemory(BinaryData::bypass_icon_png, BinaryData::bypass_icon_pngSize);
    bypassButton.setImageSize(20, 20);
    bypassButton.setImage(bypassIcon, bypassIcon, bypassIcon, juce::Colours::grey.withAlpha(1.0f), juce::Colours::red);
}



PresetPanel::~PresetPanel()
{
    previousPresetButton.getButton().removeListener(this);
    nextPresetButton.getButton().removeListener(this);
    saveButton.getButton().removeListener(this);
    deleteButton.getButton().removeListener(this);
    bypassButton.getButton().removeListener(this);
    presetList.removeListener(this);

    presetList.setLookAndFeel(nullptr);
}

//==============================================================================
// LAYOUT
//==============================================================================

void PresetPanel::paint(juce::Graphics& g)
{  
    // PANEL BACKGROUND
    auto panelBg = juce::ImageCache::getFromMemory(BinaryData::panelbg_texture_4_png, BinaryData::panelbg_texture_4_pngSize);
    g.drawImage(panelBg, 0, 0, getWidth(), getHeight(), 0, 0, panelBg.getWidth(), panelBg.getHeight());
    
    // OUTLINE
    auto panelBounds = getLocalBounds().toFloat().reduced(0.3f);
    auto panelStroke = panelBounds;

    g.setColour(juce::Colours::white.withAlpha(0.6f));
    g.drawRoundedRectangle(panelBounds, 5.0f, 0.2f);


    //==============================================================================
    // LOGO
    //==============================================================================

    const float scaleFactor{ 0.09f };

    auto logo = juce::ImageCache::getFromMemory(BinaryData::lyre_logo_4_png, BinaryData::lyre_logo_4_pngSize);
    int destWidth = logo.getWidth() * scaleFactor;
    int destHeight = logo.getHeight() * scaleFactor;
    int offset = 5;

    float horizontalPosition = logoBounds.getRight() - destWidth;
    //float verticalPosition = logoBounds.getBottom() - destHeight;
    float verticalPosition = getLocalBounds().getCentreY() - offset;

    g.drawImage(logo, horizontalPosition, verticalPosition, destWidth, destHeight, 0, 0, logo.getWidth(), logo.getHeight());
}

void PresetPanel::resized()
{
    auto bounds = getLocalBounds();

    previousPresetButton.setTopLeftPosition(2, -14);
    presetList.setBounds(previousPresetButton.getRight() + 5, 7, 120, 25 );
    nextPresetButton.setTopLeftPosition(presetList.getRight() + 5, previousPresetButton.getY());
    saveButton.setTopLeftPosition(nextPresetButton.getRight(), previousPresetButton.getY() + 5);
    deleteButton.setTopLeftPosition(saveButton.getRight(), previousPresetButton.getY() + 5);

    bypassButton.setTopLeftPosition(675, -9);

    //==============================================================================
    // LOGO
    //==============================================================================

    // Sizing
    const int logoWidth = 80;
    const int logoHeight = 40;

    // Positioning
    logoBounds = juce::Rectangle<int>(bounds.getRight() - 150, bounds.getBottom() - logoHeight, logoWidth, logoHeight);

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