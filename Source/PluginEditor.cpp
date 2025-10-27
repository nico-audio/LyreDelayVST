/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

GDelayAudioProcessorEditor::GDelayAudioProcessorEditor (GDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    delayGroup.setText("Delay");
    delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    delayGroup.addAndMakeVisible(delayTimeKnob);
    delayGroup.addAndMakeVisible(feedbackKnob);
    delayGroup.addAndMakeVisible(stereoKnob);
    addAndMakeVisible(delayGroup);

    grainGroup.setText("Feedback-GrainPlaceholder");
    grainGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    grainGroup.addAndMakeVisible(lowCutKnob);
    grainGroup.addAndMakeVisible(highCutKnob);
    addAndMakeVisible(grainGroup);
    
    outputGroup.setText("Output");
    outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    addAndMakeVisible(gainKnob);
    addAndMakeVisible(mixKnob);
    addAndMakeVisible(outputGroup);

    addAndMakeVisible(audioProcessor.waveViewer);
    audioProcessor.waveViewer.setColours(juce::Colour (252, 209, 253), juce::Colour (12, 12, 20));

    setSize (660, 470);

    // gain track color override
    gainKnob.slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour (0, 198, 204));

    setLookAndFeel(&mainLF);
}

GDelayAudioProcessorEditor::~GDelayAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void GDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // noise background texture
    auto noise = juce::ImageCache::getFromMemory(BinaryData::bg_texture_png, BinaryData::bg_texture_pngSize);
    auto fillType = juce::FillType(noise, juce::AffineTransform::scale(0.5f));
    g.setFillType(fillType);
    g.fillRect(getLocalBounds());

    // top logo image
    const float scaleFactor { 0.15f };

    auto rect = getLocalBounds().withHeight(40);
    g.setColour(Colors::header);
    g.fillRect(rect);
    auto image = juce::ImageCache::getFromMemory(BinaryData::GDelay_logo_png, BinaryData::GDelay_logo_pngSize);
    int destWidth = image.getWidth() * scaleFactor;
    int destHeight = image.getHeight() * scaleFactor;

    float verticalPosition = rect.getY() + (rect.getHeight() - destHeight) / 2;

    g.drawImage(image, getWidth() / 2 - destWidth / 2, verticalPosition, destWidth, destHeight, 0, 0, image.getWidth(), image.getHeight());
}

void GDelayAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    const int groupSpacing { 10 };
    const int delayGroupWidth { 150 };
    const int outputGroupWidth { 150 };
    const int delayLeftEdge { 10 };
    const int outputRightEdge { 160 };
    const int waveViewerHeight{ 50 };
    const int waveViewerWidth { 630 };
    
    int topMargin = 100;
    int height = bounds.getHeight() - 110;

    // Position the groups
    delayGroup.setBounds(delayLeftEdge, topMargin, delayGroupWidth, height);
    
    outputGroup.setBounds(bounds.getWidth() - outputRightEdge, topMargin, outputGroupWidth, height);
    
    grainGroup.setBounds(delayGroup.getRight() + groupSpacing, topMargin,
        outputGroup.getX() - delayGroup.getRight() - 20,
        height);
    
    // Position the knobs inside the groups
    delayTimeKnob.setTopLeftPosition(20, 20);
    feedbackKnob.setTopLeftPosition(delayTimeKnob.getX(), delayTimeKnob.getBottom() + 10);
    stereoKnob.setTopLeftPosition(feedbackKnob.getX(), feedbackKnob.getBottom() + 10);

    lowCutKnob.setTopLeftPosition(80, 210);
    highCutKnob.setTopLeftPosition(lowCutKnob.getRight() + 20, lowCutKnob.getY());

    mixKnob.setTopLeftPosition(520, 120);
    gainKnob.setTopLeftPosition(mixKnob.getX(), mixKnob.getBottom() + 10);
    
    // Audio visualizer
    audioProcessor.waveViewer.setBounds(15, 45, waveViewerWidth, waveViewerHeight);
}
