/*
  ==============================================================================

    RotaryKnob.h
    Created: 17 Sep 2025 10:36:44am
    Author:  Nico V.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RotaryKnob  : public juce::Component
{
public:
    enum class KnobSize { Small, Medium, Large };

    RotaryKnob(const juce::String& text,
               juce::AudioProcessorValueTreeState& apvts,
               const juce::ParameterID& parameterID,
               bool drawFromMiddle = false, 
               KnobSize size = KnobSize::Medium);
    ~RotaryKnob() override;

    void setKnobSize(KnobSize size);
    void resized() override;

    juce::Slider slider;
    juce::Label label;

    juce::AudioProcessorValueTreeState::SliderAttachment attachment;

private:
    KnobSize knobSize = KnobSize::Medium;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
