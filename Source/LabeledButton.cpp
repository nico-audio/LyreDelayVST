/*
  ==============================================================================

    LabeledButton.cpp
    Created: 23 Jan 2026 3:40:57pm
    Author:  Nico V.

  ==============================================================================
*/

#include "LabeledButton.h"
#include "LookAndFeel.h"

LabeledButton::LabeledButton(const juce::String& labelText, const juce::String& buttonText, juce::AudioProcessorValueTreeState& apvts,
    const juce::ParameterID& parameterID, juce::Rectangle<int> buttonBounds) : attachment(apvts, parameterID.getParamID(), button)
{
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setMinimumHorizontalScale(1.0f);
    addAndMakeVisible(label);

    button.setButtonText(buttonText);
    button.setClickingTogglesState(true);
    addAndMakeVisible(button);

    // component size
    setSize(buttonBounds.getWidth(), buttonBounds.getHeight() + 24);

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
    //label.setBounds(0, 0, 75, 20);

    //button.setBounds(label.getX() + 25, label.getY() + 20, 40, 20);

    label.setSize(75, 20);
    button.setSize(40, 20);
    
    auto labelBounds = label.getBounds();
    auto buttonBounds = button.getBounds();

    // Compute the centre point for button
    juce::Point<int> newCentre(
        labelBounds.getCentreX(),     // same horizontal centre as A
        labelBounds.getBottom() + buttonBounds.getHeight() / 2   // below A
    );

    // Move button's rectangle so its centre matches newCentre
    auto newBounds = buttonBounds.withCentre(newCentre);

    button.setBounds(newBounds);
}