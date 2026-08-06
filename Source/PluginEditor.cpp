/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GUI/LayoutHelper.h"
#include <melatonin_inspector/melatonin_inspector.h>

GDelayAudioProcessorEditor::GDelayAudioProcessorEditor (GDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), meter (p.levelL, p.levelR), presetPanel(p.getPresetManager(), p)
{
    //==============================================================================
    // DELAY GROUP
    //==============================================================================

    delayGroup.setText("Delay");
    delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    delayGroup.addAndMakeVisible(delayTimeKnob);
    delayGroup.addChildComponent(delayNoteKnob);
    delayGroup.addAndMakeVisible(feedbackKnob);
    delayGroup.addAndMakeVisible(stereoKnob);
    delayGroup.addAndMakeVisible(lowCutKnob);
    delayGroup.addAndMakeVisible(highCutKnob);
    delayGroup.addAndMakeVisible(tempoSyncButton);
    addAndMakeVisible(delayGroup);

    //==============================================================================
    // GRANULAR GROUP
    //==============================================================================

    grainGroup.setText("Granular");
    grainGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    grainGroup.addAndMakeVisible(granularToggleButton);
    grainGroup.addAndMakeVisible(grainSize);
    grainGroup.addAndMakeVisible(grainPitch);
    grainGroup.addAndMakeVisible(grainDensity);
    grainGroup.addAndMakeVisible(textureKnob);
    grainGroup.addAndMakeVisible(randomizerButton);
    addAndMakeVisible(grainGroup);
    
    //==============================================================================
    // OUTPUT GROUP
    //==============================================================================

    outputGroup.setText("Output");
    outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    addAndMakeVisible(meter);
    addAndMakeVisible(gainKnob);
    addAndMakeVisible(mixKnob);
    addAndMakeVisible(outputGroup);

    //==============================================================================
    // DISPLAY PANELS
    //==============================================================================

    addAndMakeVisible(audioProcessor.audioVisualiser);
    addAndMakeVisible(presetPanel);

    //==============================================================================
    // IMAGE BUTTONS
    //==============================================================================

    auto randomizerIcon = juce::ImageCache::getFromMemory(BinaryData::randomizer_dice_white_png, BinaryData::randomizer_dice_white_pngSize);
    randomizerButton.setClickingTogglesState(false);
    randomizerButton.setImage(randomizerIcon, randomizerIcon, randomizerIcon, juce::Colour(206, 148, 92), juce::Colour(206, 148, 92));
    randomizerButton.getButton().onClick = [this]
    {
        audioProcessor.randomizeParams();
    };

    auto granularIcon = juce::ImageCache::getFromMemory(BinaryData::granular_button_white_png, BinaryData::granular_button_white_pngSize);
    granularToggleButton.setImage(granularIcon, granularIcon, granularIcon, juce::Colour(206, 148, 92), juce::Colour(206, 148, 92));
    granularToggleButton.setImageSize(25, 25);

    auto syncIcon = juce::ImageCache::getFromMemory(BinaryData::sync_button_white_png, BinaryData::sync_button_white_pngSize);
    tempoSyncButton.setImage(syncIcon, syncIcon, syncIcon, juce::Colour(206, 148, 92), juce::Colour(206, 148, 92));
    tempoSyncButton.setImageSize(20, 20);

    //==============================================================================

    // Dev module
    //inspector = std::make_unique<melatonin::Inspector>(*this);
    //inspector->setVisible(true);

    setSize (750, 520);

    // gain track color override
    gainKnob.slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour (42, 192, 8));

    setLookAndFeel(&mainLF);

    updateDelayKnobs(audioProcessor.params.tempoSyncParam->get());
    updateButtonEnabled(audioProcessor.params.granularToggleParam->get());
    
    audioProcessor.params.tempoSyncParam->addListener(this);
    audioProcessor.params.granularToggleParam->addListener(this);
}

//==============================================================================

GDelayAudioProcessorEditor::~GDelayAudioProcessorEditor()
{
    audioProcessor.params.tempoSyncParam->removeListener(this);
    audioProcessor.params.granularToggleParam->removeListener(this);
    setLookAndFeel(nullptr);
}

void GDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    //==============================================================================
    // BACKGROUND
    //==============================================================================

    auto bgTexture = juce::ImageCache::getFromMemory(BinaryData::bg_texture_png, BinaryData::bg_texture_pngSize);
    g.drawImage(bgTexture, 0, 0, getWidth(), getHeight(), 25, 10, bgTexture.getWidth() * 0.8, bgTexture.getHeight() * 0.9);

}

void GDelayAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    //==============================================================================
    // GROUPS
    //==============================================================================
    
    int topMargin = 80;
    int height = bounds.getHeight() - 130;
    
    const int groupSpacing { 10 };
    const int delayGroupWidth { 300 };
    const int outputGroupWidth { 190 };
    const int delayLeftEdge { 10 };
    const int outputRightEdge { 200 };
    const int waveViewerHeight{ 70 };
    const int waveViewerWidth { 730 };

    delayGroup.setBounds(delayLeftEdge, topMargin, delayGroupWidth, height);
    outputGroup.setBounds(bounds.getWidth() - outputRightEdge, topMargin, outputGroupWidth, height);
    grainGroup.setBounds(delayGroup.getRight() + groupSpacing, topMargin, outputGroup.getX() - delayGroup.getRight() - 20, height);
    
    // Position the knobs inside the groups
    delayTimeKnob.setTopLeftPosition(115, 20);
    tempoSyncButton.setTopLeftPosition(30 , 45);
    delayNoteKnob.setTopLeftPosition(delayTimeKnob.getX(), delayTimeKnob.getY());

    feedbackKnob.setTopLeftPosition(115, delayTimeKnob.getBottom() + 10);
    stereoKnob.setTopLeftPosition(30, 260);

    lowCutKnob.setTopLeftPosition(stereoKnob.getRight() + 20, 260);
    highCutKnob.setTopLeftPosition(lowCutKnob.getRight() + 20, lowCutKnob.getY());

    granularToggleButton.setTopLeftPosition(50, 20);
    grainSize.setTopLeftPosition(20, 75);
    grainPitch.setTopLeftPosition(20, 200);
    grainDensity.setTopLeftPosition(110, 75);
    textureKnob.setTopLeftPosition(110, 200);
    randomizerButton.setTopLeftPosition(50, 318);

    mixKnob.setTopLeftPosition(570, 120);
    gainKnob.setTopLeftPosition(mixKnob.getX(), mixKnob.getBottom() + 10);
    meter.setBounds(630, 135, 96, 220);

    //==============================================================================
    // DISPLAY PANEL
    //==============================================================================
    
    audioProcessor.audioVisualiser.setBounds(9, 3, waveViewerWidth, waveViewerHeight);

    presetPanel.setBounds(12, bounds.getBottom() - 45, 730, 40);
}

//==============================================================================

void GDelayAudioProcessorEditor::parameterValueChanged(int parameterIndex, float newValue)
{
    auto updateUI = [this, parameterIndex, newValue]
    {
        if (parameterIndex == audioProcessor.params.tempoSyncParam->getParameterIndex())
        {
            updateDelayKnobs(newValue != 0.0f);
        }
        if (parameterIndex == audioProcessor.params.granularToggleParam->getParameterIndex())
        {
            bool enabled = (newValue != 0.0f);
            randomizerButton.setEnabled(enabled);
        }
    };

    if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        updateUI();
    else
        juce::MessageManager::callAsync(updateUI);
}

void GDelayAudioProcessorEditor::updateDelayKnobs(bool tempoSyncActive)
{
    delayTimeKnob.setVisible(!tempoSyncActive);
    delayNoteKnob.setVisible(tempoSyncActive);
}

void GDelayAudioProcessorEditor::updateButtonEnabled(bool granularEnabled)
{
    randomizerButton.setEnabled(granularEnabled);
}
