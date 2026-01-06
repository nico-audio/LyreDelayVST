/*
  ==============================================================================

    LevelMeter.cpp
    Created: 12 Dec 2025 2:52:48pm
    Author:  Nico V.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LevelMeter.h"
#include "LookAndFeel.h"

Bulb::Bulb(const juce::Colour& c) : colour(c) {}

void Bulb::paint(juce::Graphics& g)
{
    // Choose fill colour depending on state
    if (isOn)
        g.setColour(colour);
    else
        g.setColour(juce::Colours::black);

    const auto delta = 3.5f;
    const auto bounds = getLocalBounds().toFloat().reduced(delta);
    const auto side = juce::jmin(bounds.getWidth(), bounds.getHeight());

    juce::Rectangle<float> bulbBounds = getLocalBounds().toFloat().withSizeKeepingCentre(side, side);

    // Fill bulb
    g.fillEllipse(bulbBounds);

    // Outline
    g.setColour(juce::Colours::black);
    g.drawEllipse(bulbBounds, 1.0f);

    // Glow effect if "on"
    if (isOn)
    {
        g.setGradientFill(
            juce::ColourGradient{
                colour.withAlpha(0.3f), bulbBounds.getCentre(),
                colour.withLightness(1.5f).withAlpha(0.0f), {}, true
            }
        );
        g.fillEllipse(bulbBounds.expanded(delta));
    }
}

void Bulb::setState(bool state)
{
    isOn = state;
    repaint();
}

LevelMeter::LevelMeter(Measurement& measurementL_, Measurement& measurementR_)
                       : measurementL(measurementL_), measurementR(measurementR_), dbLevelL(clampdB), dbLevelR(clampdB)
{
    startTimerHz(refreshRate);
    decay = 1.0f - std::exp(-1.0f / (float(refreshRate) * timeConstant));
}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds();

    // Map dB values into [0,1] range
    const auto valueL = juce::jmap(dbLevelL, mindB, maxdB, 0.0f, 1.0f);
    const auto valueR = juce::jmap(dbLevelR, mindB, maxdB, 0.0f, 1.0f);

    // Light bulbs based on normalized values
    for (int i = 0; i < totalNumberOfBulbs; ++i)
    {
        bulbsL[i]->setState(valueL >= float(i + 1) / totalNumberOfBulbs);
        bulbsR[i]->setState(valueR >= float(i + 1) / totalNumberOfBulbs);
    }

    drawLabel(g);
}

void LevelMeter::resized()
{
    juce::ColourGradient gradientL{ Colors::LevelMeter::levelOK , getLocalBounds().getBottomLeft().toFloat(),
                                    Colors::LevelMeter::levelLoud, getLocalBounds().getTopLeft().toFloat(), false };
    gradientL.addColour(0.5, Colors::LevelMeter::levelWarning);

    // Divide height into equal segments
    const auto bulbHeight = getLocalBounds().getHeight() / totalNumberOfBulbs;
    
    // Split bounds into left and right halves 
    auto fullBounds = getLocalBounds();
    auto boundsL = fullBounds.removeFromLeft(getWidth() / 2); auto boundsR = fullBounds;
    
    // Value to move each channel's bulbs inwards. Increase to reduce the gap.
    const int offset = 13; 
    
    bulbsL.clear();
    bulbsR.clear();

    // Left channel bulbs
    for (int i = 0; i < totalNumberOfBulbs; ++i)
    {
        auto bulb = std::make_unique<Bulb>(gradientL.getColourAtPosition(double(i) / totalNumberOfBulbs));
        addAndMakeVisible(bulb.get());
        
        //bulb bounds
        auto segment = boundsL.removeFromBottom(bulbHeight);
        bulb->setBounds(segment.translated(offset, 0).withSizeKeepingCentre(18, 18));
        bulbsL.push_back(std::move(bulb));
    }

    // Right channel bulbs
    for (int i = 0; i < totalNumberOfBulbs; ++i)
    {
        auto bulb = std::make_unique<Bulb>(gradientL.getColourAtPosition(double(i) / totalNumberOfBulbs));
        addAndMakeVisible(bulb.get());

        //bulb bounds
        auto segment = boundsR.removeFromBottom(bulbHeight);
        bulb->setBounds(segment.translated(-offset, 0).withSizeKeepingCentre(18, 18));
        bulbsR.push_back(std::move(bulb));
    }

    maxPos = 12.0f;
    minPos = float(getHeight()) - 8.0f;
}

void LevelMeter::timerCallback()
{
    updateLevel(measurementL.readAndReset(), levelL, dbLevelL);
    updateLevel(measurementR.readAndReset(), levelR, dbLevelR);

    repaint();

    //DBG("left: " << measurementL.readAndReset() << ", right: " << measurementR.readAndReset());
}



void LevelMeter::drawLabel(juce::Graphics& g)
{
    g.setFont(Fonts::getPatopian(10.0f));

    for (float db = maxdB; db >= mindB; db -= stepdB)
    {
        // Map dB to vertical position
        const float proportion = (db - mindB) / (maxdB - mindB);
        const int y = int(minPos - proportion * (minPos - maxPos));

        // Tick label
        g.setColour(Colors::LevelMeter::tickLabel);
        g.drawSingleLineText(juce::String(int(db)), getWidth() - 10, y + 2, juce::Justification::right);
    }
}

// One pole filter smoothing
void LevelMeter::updateLevel(float newLevel, float& smoothedLevel, float& leveldB) const
{
    if (newLevel > smoothedLevel) {
        smoothedLevel = newLevel;
        // instantaneous attack
    }
    else {
        smoothedLevel += (newLevel - smoothedLevel) * decay;
    }
    if (smoothedLevel > clampLevel) {
        leveldB = juce::Decibels::gainToDecibels(smoothedLevel);
    }
    else {
        leveldB = clampdB;
    }
}
