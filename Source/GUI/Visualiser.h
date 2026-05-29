/*
  ==============================================================================

    Visualiser.h
    Created: 29 May 2026 10:16:16am
    Author:  Nico V.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

//==============================================================================
/*
*/
class Visualiser  : public juce::AudioVisualiserComponent
{
public:
    Visualiser();
    ~Visualiser() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Visualiser)
};
