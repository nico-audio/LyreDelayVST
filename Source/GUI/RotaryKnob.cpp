/*
  ==============================================================================

    RotaryKnob.cpp
    Created: 17 Sep 2025 10:36:44am
    Author:  Nico V.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "LookAndFeel.h"


RotaryKnob::RotaryKnob(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, 
                       const juce::ParameterID& parameterID, bool drawFromMiddle, KnobSize size)
    : attachment(apvts, parameterID.getParamID(), slider), knobSize(size)
{
    // slider
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(slider);

    // label
    label.setText(text, juce::NotificationType::dontSendNotification);;
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>(0));
    addAndMakeVisible(label);

    setLookAndFeel(RotaryKnobLookAndFeel::get());

    // change default start and end angles
    float pi = juce::MathConstants<float>::pi;
    slider.setRotaryParameters(1.25f * pi, 2.75f * pi, true);

    // draw track - draw from middle
    slider.getProperties().set("drawFromMiddle", drawFromMiddle);

    resized();
}

RotaryKnob::~RotaryKnob()
{
}

void RotaryKnob::resized()
{
    int labelHeight = 20;
    int textBoxHeight = 16;
    int sliderDiameter = 0;

    switch (knobSize)
    {
        case KnobSize::Small:
            sliderDiameter = 67;
            break;
        case KnobSize::Medium:
            sliderDiameter = 80;
            break;
        case KnobSize::Large:
            sliderDiameter = 100;
            break;
    }

    int totalWidth = sliderDiameter;
    int totalHeight = labelHeight + sliderDiameter + textBoxHeight;

    // Component size
    setSize(totalWidth, totalHeight);

    // Position the label
    label.setBounds(0, 0, totalWidth, labelHeight);

    // Position the slider and text box
    slider.setBounds(0, labelHeight, totalWidth, sliderDiameter + textBoxHeight);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, totalWidth, textBoxHeight);

}

void RotaryKnob::setKnobSize(KnobSize size)
{
    knobSize = size;
    resized();
}
