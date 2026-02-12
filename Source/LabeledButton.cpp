/*
  ==============================================================================

    LabeledButton.cpp
    Created: 23 Jan 2026 3:40:57pm
    Author:  Nico V.

  ==============================================================================
*/

#include "LabeledButton.h"
#include "LookAndFeel.h"
#include "LayoutHelper.h"

LabeledButton::LabeledButton(const juce::String& labelText, const juce::String& buttonText, juce::AudioProcessorValueTreeState& apvts,
    const juce::ParameterID& parameterID, ButtonSize size) : attachment(apvts, parameterID.getParamID(), button), buttonSize(size)
{
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setMinimumHorizontalScale(1.0f);
    addAndMakeVisible(label);

    button.setButtonText(buttonText);
    button.setClickingTogglesState(true);
    addAndMakeVisible(button);

    // adjust component size based on button size
    int w = 0;
    int h = 0;

    switch (buttonSize)
    {
        case ButtonSize::Small:  w = 65; h = 45; break;
        case ButtonSize::Medium: w = 110; h = 55; break;
        case ButtonSize::Large:  w = 200; h = 70; break;
    }

    setSize(w, h);

    jassert(apvts.getParameter(parameterID.getParamID()) != nullptr);

}

LabeledButton::~LabeledButton()
{
}

void LabeledButton::setClickingTogglesState(bool shouldToggle)
{
    button.setClickingTogglesState(shouldToggle);
}

void LabeledButton::setToggleState(bool state,
    juce::NotificationType nt)
{
    button.setToggleState(state, nt);
}

bool LabeledButton::getToggleState() const
{
    return button.getToggleState();
}


void LabeledButton::resized()
{
    switch (buttonSize)
    {
        case ButtonSize::Small:
            label.setBounds(0, 0, 60, 20);
            button.setBounds(0, 25, 40, 20);
            centreBelow(label, button, 0);
            break;

        case ButtonSize::Medium:
            label.setBounds(0, 0, 110, 20);
            button.setBounds(0, 0, 50, 25);
            centreBelow(label, button, 0);
            break;

        case ButtonSize::Large:
            label.setBounds(0, 0, 200, 20);
            button.setBounds(0, 0, 100, 30);
            centreBelow(label, button, 0);
            break;
    }
}

void LabeledButton::setButtonSize(ButtonSize size) 
{ 
    buttonSize = size; 
    resized(); 
}