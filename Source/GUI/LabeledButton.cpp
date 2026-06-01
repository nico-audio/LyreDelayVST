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

//=============================================================================
// CONSTRUCTOR/ DESTRUCTOR
//=============================================================================

LabeledButton::LabeledButton(const juce::String& labelText, const juce::String& buttonText, juce::AudioProcessorValueTreeState& apvts,
    const juce::ParameterID& parameterID, ButtonType type, ButtonSize size) : buttonType(type), buttonSize(size)
{

    if (type == ButtonType::Text){
        auto* txtButton = new juce::TextButton(buttonText);
        txtButton->setClickingTogglesState(true);
        button.reset(txtButton);
    }
    else{
        auto* imgButton = new juce::ImageButton();
        imgButton->setClickingTogglesState(true);
        button.reset(imgButton);
    }

    addAndMakeVisible(*button);

    // Label
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setMinimumHorizontalScale(1.0f);
    addAndMakeVisible(label);

    jassert(button != nullptr);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, parameterID.getParamID(),*button);

    // adjust component size based on button size
    int w = 0;
    int h = 0;

    switch (buttonSize)
    {
        case ButtonSize::Small:  w = 60; h = 55; break;
        case ButtonSize::Medium: w = 65; h = 45; break;
        case ButtonSize::Large:  w = 110; h = 55; break;
    }

    setSize(w, h);

    jassert(apvts.getParameter(parameterID.getParamID()) != nullptr);
}

LabeledButton::~LabeledButton()
{
}

//==============================================================================
// BUTTON FORWARDING
//==============================================================================

void LabeledButton::setClickingTogglesState(bool shouldToggle)
{
    button->setClickingTogglesState(shouldToggle);
}

void LabeledButton::setToggleState(bool state, juce::NotificationType nt)
{
    button->setToggleState(state, nt);
}

bool LabeledButton::getToggleState() const
{
    return button->getToggleState();
}

//==============================================================================
// IMAGE
//==============================================================================

void LabeledButton::setImage(const juce::Image& normal,
                             const juce::Image& over,
                             const juce::Image& down)
{
    if (auto* imgButton = dynamic_cast<juce::ImageButton*>(button.get())){
        imgButton->setImages(false, true, true,
            normal, 1.0f, juce::Colours::grey,
            over, 1.0f, juce::Colours::white,
            down, 1.0f, juce::Colour(206, 148, 92), 0.0f);
    }
}

//==============================================================================
// LAYOUT
//==============================================================================

void LabeledButton::resized()
{
    switch (buttonSize)
    {
        case ButtonSize::Small:
            label.setBounds(0, 0, 60, 20);
            button->setBounds(0, 25, 25, 25);
            centreBelow(label, *button, 0);
            break;

        case ButtonSize::Medium:
            label.setBounds(0, 0, 60, 20);
            button->setBounds(0, 25, 40, 20);
            centreBelow(label, *button, 0);
            break;

        case ButtonSize::Large:
            label.setBounds(0, 0, 110, 15);
            button->setBounds(0, 25, 50, 25);
            centreBelow(label, *button, 0);
            break;
    }

    if (buttonType == ButtonType::Image) {
        button->setBounds(getLocalBounds().withSizeKeepingCentre(imageWidth, imageHeight));
    }
}

void LabeledButton::setButtonSize(ButtonSize size) 
{ 
    buttonSize = size; 
    resized(); 
}

void LabeledButton::setImageSize(int width, int height) {
    imageWidth = width;
    imageHeight = height;
    resized();
}