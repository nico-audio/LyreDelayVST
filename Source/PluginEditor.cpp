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
    addAndMakeVisible(gainKnob);
    setSize (500, 330);
}

GDelayAudioProcessorEditor::~GDelayAudioProcessorEditor()
{
}

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
    gainKnob.setTopLeftPosition(215, 120);
}
