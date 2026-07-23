/*
  ==============================================================================

    FactoryPreset.h
    Created: 23 Jul 2026 11:08:13am
    Author:  Nico V.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct FactoryPreset
{
    const char* name;
    const void* data;
    const int size;
};

const FactoryPreset* findFactoryPreset(const juce::String& name);

juce::StringArray getFactoryPresetNames();