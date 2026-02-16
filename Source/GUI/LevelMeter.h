/*
  ==============================================================================

    LevelMeter.h
    Created: 12 Dec 2025 2:52:48pm
    Author:  Nico V.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Measurement.h"

class Bulb : public juce::Component
{
public:
    // Construct a bulb with a given colour
    explicit Bulb(const juce::Colour& c);

    void paint(juce::Graphics& g) override;
    void setState(bool state);

private:
    bool isOn = false;
    juce::Colour colour{};   // Base colour of bulb
};

class LevelMeter  : public juce::Component, private juce::Timer
{
public:
    LevelMeter(Measurement& measurementL, Measurement& measurementR);
    ~LevelMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    int positionForLevel(float dbLevel) const noexcept
    {
        return int(std::round(juce::jmap(dbLevel, maxdB, mindB, maxPos, minPos)));
    }
    
    void drawLabel(juce::Graphics& g);
    void updateLevel(float newLevel, float& smoothedLevel, float& leveldB) const;

    // Store measurements from processor
    Measurement& measurementL;
    Measurement& measurementR;

    // Meter limits and marks
    const int totalNumberOfBulbs = 12;
    static constexpr float maxdB = 6.0f;
    static constexpr float mindB = -60.0f;
    static constexpr float stepdB = 6.0f;

    // y-coordinates for position
    float maxPos = 0.0f;
    float minPos = 0.0f;

    static constexpr float clampdB = -120.0f;
    static constexpr float clampLevel = 0.000001f; // -120 dB

    float dbLevelL;
    float dbLevelR;

    static constexpr int refreshRate = 60;

    float decay = 0.0f;
    float levelL = clampLevel;
    float levelR = clampLevel;

    // Smoothing time constant (seconds)
    float timeConstant = 0.2f;

    // Bulb containers
    std::vector<std::unique_ptr<Bulb>> bulbsL;
    std::vector<std::unique_ptr<Bulb>> bulbsR;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
