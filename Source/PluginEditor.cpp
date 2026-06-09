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
    : AudioProcessorEditor (&p), audioProcessor (p), meter (p.levelL, p.levelR), presetPanel(p.apvts)
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
    // BUTTONS
    //==============================================================================

    auto bypassIcon = juce::ImageCache::getFromMemory(BinaryData::Bypassplaceholder_png, BinaryData::Bypassplaceholder_pngSize);
    bypassButton.setClickingTogglesState(true);
    bypassButton.setBounds(0, 0, 20, 20);
    bypassButton.setImages(
        false, true, true,
        bypassIcon, 1.0f, juce::Colours::white,
        bypassIcon, 1.0f, juce::Colours::white,
        bypassIcon, 1.0f, juce::Colours::grey,
        0.0f);
    addAndMakeVisible(bypassButton);

    auto randomizerIcon = juce::ImageCache::getFromMemory(BinaryData::randomizer_dice_png, BinaryData::randomizer_dice_pngSize);
    randomizerButton.setClickingTogglesState(false);
    randomizerButton.setImage(randomizerIcon, randomizerIcon, randomizerIcon);
    randomizerButton.getButton().onClick = [this]
    {
        audioProcessor.randomizeParams();
    };

    auto granularIcon = juce::ImageCache::getFromMemory(BinaryData::grainicon_png, BinaryData::grainicon_pngSize);
    granularToggleButton.setImage(granularIcon, granularIcon, granularIcon);
    granularToggleButton.setImageSize(25, 25);

    auto syncIcon = juce::ImageCache::getFromMemory(BinaryData::syncbutton_png, BinaryData::syncbutton_pngSize);
    tempoSyncButton.setImage(syncIcon, syncIcon, syncIcon);
    tempoSyncButton.setImageSize(20, 20);

    //==============================================================================

    // Dev module
    inspector = std::make_unique<melatonin::Inspector>(*this);
    inspector->setVisible(true);

    setSize (720, 490);

    // gain track color override
    gainKnob.slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour (42, 192, 8));

    setLookAndFeel(&mainLF);
    setLookAndFeel(&buttonLF);

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
    auto bgTexture = juce::ImageCache::getFromMemory(BinaryData::Lyre_BG_Texture_brushed_metal_png, BinaryData::Lyre_BG_Texture_brushed_metal_pngSize);
    g.drawImageAt(bgTexture, 0, 0);
    auto fillType = juce::FillType(bgTexture, juce::AffineTransform::scale(0.5f));
    g.setFillType(fillType);
    g.fillRect(getLocalBounds());

    //==============================================================================
    // LOGO
    //==============================================================================

    const float scaleFactor { 0.15f };

    auto image = juce::ImageCache::getFromMemory(BinaryData::lyre_logo_1_png, BinaryData::lyre_logo_1_pngSize);
    int destWidth = image.getWidth() * scaleFactor;
    int destHeight = image.getHeight() * scaleFactor;

    float horizontalPosition = logoBounds.getRight() - destWidth;
    float verticalPosition = logoBounds.getBottom() - destHeight;

    g.drawImage(image, horizontalPosition , verticalPosition, destWidth, destHeight, 0, 0, image.getWidth(), image.getHeight());

}

void GDelayAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    //==============================================================================
    // LOGO
    //==============================================================================
    
    // Sizing
    const int logoWidth = 100;
    const int logoHeight = 40;

    // Positioning
    logoBounds = juce::Rectangle<int>(bounds.getWidth() / 2 , bounds.getBottom() - logoHeight, logoWidth, logoHeight);

    //==============================================================================
    // GROUPS
    //==============================================================================
    
    int topMargin = 75;
    int height = bounds.getHeight() - 110;
    
    const int groupSpacing { 10 };
    const int delayGroupWidth { 300 };
    const int outputGroupWidth { 170 };
    const int delayLeftEdge { 10 };
    const int outputRightEdge { 180 };
    const int waveViewerHeight{ 70 };
    const int waveViewerWidth { 700 };

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

    mixKnob.setTopLeftPosition(550, 120);
    gainKnob.setTopLeftPosition(mixKnob.getX(), mixKnob.getBottom() + 10);
    meter.setBounds(612, 135, 96, 220);

    bypassButton.setTopLeftPosition(bounds.getRight() - bypassButton.getWidth() - 10, 463);

    
    //==============================================================================
    // DISPLAY PANEL
    //==============================================================================
    
    audioProcessor.audioVisualiser.setBounds(9, 3, waveViewerWidth, waveViewerHeight);

    presetPanel.setBounds(0, bounds.getBottom() - 60, 350, 60);
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
