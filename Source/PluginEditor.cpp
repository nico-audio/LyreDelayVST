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
    : AudioProcessorEditor (&p), audioProcessor (p), meter (p.levelL, p.levelR)
{
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

    grainGroup.setText("Granular");
    grainGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    grainGroup.addAndMakeVisible(granularToggleButton);
    grainGroup.addAndMakeVisible(grainSize);
    grainGroup.addAndMakeVisible(grainPitch);
    addAndMakeVisible(grainGroup);
    
    outputGroup.setText("Output");
    outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    addAndMakeVisible(meter);
    addAndMakeVisible(gainKnob);
    addAndMakeVisible(mixKnob);
    addAndMakeVisible(outputGroup);

    addAndMakeVisible(audioProcessor.waveViewer);
    audioProcessor.waveViewer.setColours(Colors::AudioVisualizer::visualizerBG, Colors::AudioVisualizer::visualizerWave);

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

    // Dev module
    //inspector = std::make_unique<melatonin::Inspector>(*this);
    //inspector->setVisible(true);

    setSize (720, 490);

    // gain track color override
    gainKnob.slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour (42, 192, 8));

    setLookAndFeel(&mainLF);
    setLookAndFeel(&buttonLF);

    updateDelayKnobs(audioProcessor.params.tempoSyncParam->get());
    audioProcessor.params.tempoSyncParam->addListener(this);
}

GDelayAudioProcessorEditor::~GDelayAudioProcessorEditor()
{
    audioProcessor.params.tempoSyncParam->removeListener(this);
    setLookAndFeel(nullptr);
}

void GDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // background texture
    auto bgTexture = juce::ImageCache::getFromMemory(BinaryData::Lyre_BG_Texture_brushed_metal_png, BinaryData::Lyre_BG_Texture_brushed_metal_pngSize);
    g.drawImageAt(bgTexture, 0, 0);
    auto fillType = juce::FillType(bgTexture, juce::AffineTransform::scale(0.5f));
    g.setFillType(fillType);
    g.fillRect(getLocalBounds());

    // top logo image
    const float scaleFactor { 0.15f };

    auto rect = getLocalBounds().withHeight(40);
    g.setColour(Colors::header);
    g.fillRect(rect);
    auto image = juce::ImageCache::getFromMemory(BinaryData::lyre_logo_1_png, BinaryData::lyre_logo_1_pngSize);
    int destWidth = image.getWidth() * scaleFactor;
    int destHeight = image.getHeight() * scaleFactor;

    float verticalPosition = rect.getY() + (rect.getHeight() - destHeight) / 2;

    g.drawImage(image, getWidth() / 2 - destWidth / 2, verticalPosition, destWidth, destHeight, 0, 0, image.getWidth(), image.getHeight());
}

void GDelayAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    const int groupSpacing { 10 };
    const int delayGroupWidth { 300 };
    const int outputGroupWidth { 170 };
    const int delayLeftEdge { 10 };
    const int outputRightEdge { 180 };
    const int waveViewerHeight{ 50 };
    const int waveViewerWidth { 690 };
    
    int topMargin = 100;
    int height = bounds.getHeight() - 110;

    // Position the groups
    delayGroup.setBounds(delayLeftEdge, topMargin, delayGroupWidth, height);
    
    outputGroup.setBounds(bounds.getWidth() - outputRightEdge, topMargin, outputGroupWidth, height);
    
    grainGroup.setBounds(delayGroup.getRight() + groupSpacing, topMargin,
        outputGroup.getX() - delayGroup.getRight() - 20,
        height);
    
    // Position the knobs inside the groups
    delayTimeKnob.setTopLeftPosition(115, 20);
    tempoSyncButton.setTopLeftPosition(delayTimeKnob.getRight() + 10 , delayTimeKnob.getY() + 50);
    delayNoteKnob.setTopLeftPosition(delayTimeKnob.getX(), delayTimeKnob.getY());

    feedbackKnob.setTopLeftPosition(115, delayTimeKnob.getBottom() + 10);
    stereoKnob.setTopLeftPosition(30, 260);

    lowCutKnob.setTopLeftPosition(stereoKnob.getRight() + 20, 260);
    highCutKnob.setTopLeftPosition(lowCutKnob.getRight() + 20, lowCutKnob.getY());

    granularToggleButton.setTopLeftPosition(20, 20);
    grainSize.setTopLeftPosition(20, 75);
    grainPitch.setTopLeftPosition(20, 200);

    mixKnob.setTopLeftPosition(550, 120);
    gainKnob.setTopLeftPosition(mixKnob.getX(), mixKnob.getBottom() + 10);
    meter.setBounds(612, 135, 96, 220);

    bypassButton.setTopLeftPosition(bounds.getRight() - bypassButton.getWidth() - 10, 10);
    
    // Audio visualizer
    audioProcessor.waveViewer.setBounds(15, 45, waveViewerWidth, waveViewerHeight);
}

void GDelayAudioProcessorEditor::parameterValueChanged(int, float value)
{
    if (juce::MessageManager::getInstance()->isThisTheMessageThread()) {
        updateDelayKnobs(value != 0.0f);
    }
    else {
        juce::MessageManager::callAsync([this, value]
        {
            updateDelayKnobs(value != 0.0f);
        });
    }

    //DBG("parameter changed: " << value);
}

void GDelayAudioProcessorEditor::updateDelayKnobs(bool tempoSyncActive)
{
    delayTimeKnob.setVisible(!tempoSyncActive);
    delayNoteKnob.setVisible(tempoSyncActive);
}
