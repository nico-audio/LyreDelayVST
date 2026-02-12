/*
  ==============================================================================

    LayoutHelper.h
    Created: 28 Jan 2026 2:29:44pm
    Author:  Nico V.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// Center button with text above
static void centreBelow(juce::Label& label, juce::TextButton& button, int gap = 0)
{
    // Ensure the button has a valid size
    jassert(button.getWidth() > 0 && button.getHeight() > 0);

    auto labelBounds = label.getBounds();
    auto buttonBounds = button.getBounds();

    // Compute the centre point for button
    juce::Point<int> newCentre(
        labelBounds.getCentreX(), 
        labelBounds.getBottom() + gap + buttonBounds.getHeight() / 2
    );

    // Move button's rectangle so its centre matches newCentre
    auto newBounds = buttonBounds.withCentre(newCentre);

    button.setBounds(newBounds);
}