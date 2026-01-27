/*
  ==============================================================================

    LabeledButton.h
    Created: 23 Jan 2026 3:40:57pm
    Author:  Nico V.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LabeledButton : public juce::Component
{
public:
    LabeledButton(const juce::String& labelText, const juce::String& buttonText, juce::AudioProcessorValueTreeState& apvts,
        const juce::ParameterID& parameterID, juce::Rectangle<int> buttonBounds = { 0, 0, 80, 40 });
    ~LabeledButton() override;

    void setClickingTogglesState(bool shouldToggle);
    void setToggleState(bool shouldBeOn, juce::NotificationType notification);
    bool getToggleState() const;
    
    juce::TextButton& getButton() { return button; }

    void resized() override;

    juce::Label label;
    juce::TextButton button;
    
    juce::AudioProcessorValueTreeState::ButtonAttachment attachment;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledButton)
};