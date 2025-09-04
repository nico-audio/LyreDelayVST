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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

GDelayAudioProcessorEditor::~GDelayAudioProcessorEditor()
{
}

//==============================================================================
void GDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::blue);

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (40.0f));
    g.drawFittedText ("Delay plugin!", getLocalBounds(), juce::Justification::centred, 1);
}

void GDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
