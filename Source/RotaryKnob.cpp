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
                       const juce::ParameterID& parameterID, bool drawFromMiddle,
                       juce::Rectangle<int> sliderBounds)
    : attachment(apvts, parameterID.getParamID(), slider)
{
    // slider
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, sliderBounds.getWidth(), 16);
    slider.setBounds(sliderBounds);
    addAndMakeVisible(slider);

    // label
    label.setText(text, juce::NotificationType::dontSendNotification);;
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>(0));
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);

    // component size
    setSize(sliderBounds.getWidth(), sliderBounds.getHeight() + 24);

    setLookAndFeel(RotaryKnobLookAndFeel::get());

    // change default start and end angles
    float pi = juce::MathConstants<float>::pi;
    slider.setRotaryParameters(1.25f * pi, 2.75f * pi, true);

    // draw track - draw from middle
    slider.getProperties().set("drawFromMiddle", drawFromMiddle);
}

RotaryKnob::~RotaryKnob()
{
}

void RotaryKnob::resized()
{
    slider.setTopLeftPosition(0, 24);
    //DBG("Label bounds: " << getBounds().toString());
}
