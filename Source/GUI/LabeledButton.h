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
    enum class ButtonSize{ Small, Medium, Large };
    enum class ButtonType{ Text, Image };
    
    LabeledButton(const juce::String& labelText, const juce::String& buttonText, juce::AudioProcessorValueTreeState& apvts,
        const juce::ParameterID& parameterID, ButtonType type = ButtonType::Text, ButtonSize size = ButtonSize::Small);
    ~LabeledButton() override;

    void setClickingTogglesState(bool shouldToggle);
    void setToggleState(bool shouldBeOn, juce::NotificationType notification);
    bool getToggleState() const;
    
    juce::Button& getButton() { return *button; }

    void setButtonSize(ButtonSize size);
    void setImage(const juce::Image& normal, const juce::Image& over, const juce::Image& down);

    void resized() override;

private:
    ButtonType buttonType;
    ButtonSize buttonSize = ButtonSize::Medium;

    juce::Label label;
    std::unique_ptr<juce::Button> button;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledButton)
};