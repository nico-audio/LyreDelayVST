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
    addAndMakeVisible(delayGroup);

    grainGroup.setText("Feedback-GrainPlaceholder");
    grainGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    addAndMakeVisible(grainGroup);
    
    outputGroup.setText("Output");
    outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    addAndMakeVisible(gainKnob);
    addAndMakeVisible(mixKnob);
    addAndMakeVisible(outputGroup);

    setSize (500, 330);

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
    const int delayGroupWidth { 110 };
    const int outputGroupWidth { 150 };
    const int delayLeftEdge { 10 };
    const int outputRightEdge { 160 };
    
    int topMargin = 50;
    int height = bounds.getHeight() - 60;

    // Position the groups
    delayGroup.setBounds(delayLeftEdge, topMargin, delayGroupWidth, height);
    
    outputGroup.setBounds(bounds.getWidth() - outputRightEdge, topMargin, outputGroupWidth, height);
    
    grainGroup.setBounds(delayGroup.getRight() + groupSpacing, topMargin,
        outputGroup.getX() - delayGroup.getRight() - 20,
        height);
    
    // Position the knobs inside the groups
    delayTimeKnob.setTopLeftPosition(20, 20);
    mixKnob.setTopLeftPosition(380, 65);
    gainKnob.setTopLeftPosition(mixKnob.getX(), mixKnob.getBottom() + 10);
    
}
