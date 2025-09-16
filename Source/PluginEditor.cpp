/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GDelayAudioProcessorEditor::GDelayAudioProcessorEditor (GDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    slider.setBounds(0, 0, 70, 86);
    addAndMakeVisible(slider);

    label.setText("Gain", juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>{ 0, 0, 2, 0 });
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);

    setSize (500, 330);
}

GDelayAudioProcessorEditor::~GDelayAudioProcessorEditor()
{
}

//==============================================================================
void GDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkgrey);

    /*
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (40.0f));
    g.drawFittedText ("Delay plugin!", getLocalBounds(), juce::Justification::centred, 1);
    */
}

void GDelayAudioProcessorEditor::resized()
{
    slider.setTopLeftPosition(215, 120);
}
