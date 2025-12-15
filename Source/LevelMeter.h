/*
  ==============================================================================

    LevelMeter.h
    Created: 12 Dec 2025 2:52:48pm
    Author:  Nico V.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class LevelMeter  : public juce::Component, private juce::Timer
{
public:
    LevelMeter(std::atomic<float>& measurementL, std::atomic<float>& measurementR);
    ~LevelMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    std::atomic<float>& measurementL;
    std::atomic<float>& measurementR;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
