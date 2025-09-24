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
}

GDelayAudioProcessorEditor::~GDelayAudioProcessorEditor()
{
}

void GDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Colors::background);
}

void GDelayAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    const int groupSpacing { 10 };
    const int delayGroupWidth { 110 };
    const int outputGroupWidth { 150 };
    const int delayLeftEdge { 10 };
    const int outputRightEdge { 160 };
    
    int topMargin = 10;
    int height = bounds.getHeight() - 20;

    // Position the groups
    delayGroup.setBounds(delayLeftEdge, topMargin, delayGroupWidth, height);
    
    outputGroup.setBounds(bounds.getWidth() - outputRightEdge, topMargin, outputGroupWidth, height);
    
    grainGroup.setBounds(delayGroup.getRight() + groupSpacing, topMargin,
        outputGroup.getX() - delayGroup.getRight() - 20,
        height);
    
    // Position the knobs inside the groups
    delayTimeKnob.setTopLeftPosition(20, 20);
    mixKnob.setTopLeftPosition(380, 20);
    gainKnob.setTopLeftPosition(mixKnob.getX(), mixKnob.getBottom() + 10);
    
}
