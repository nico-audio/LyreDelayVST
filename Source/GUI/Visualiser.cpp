/*
  ==============================================================================

    Visualiser.cpp
    Created: 29 May 2026 10:16:16am
    Author:  Nico V.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Visualiser.h"

//==============================================================================
Visualiser::Visualiser() : AudioVisualiserComponent(2)
{
    setBufferSize(512);
    setSamplesPerBlock(256);
    setRepaintRate(30);
    setColours(juce::Colours::transparentBlack, Colors::AudioVisualizer::visualizerWave);
    setOpaque(false);
}

Visualiser::~Visualiser()
{
}

void Visualiser::paint (juce::Graphics& g)
{
    auto visualizerBounds = getLocalBounds().toFloat().reduced(4.0f);

    constexpr float cornerSize = 6.0f;
    constexpr float lineThickness = 1.0f;
    constexpr float glowSpread = 0.5f;

    g.setColour(Colors::AudioVisualizer::avGlow.withAlpha(0.10f));

    for (int i = 0; i <= 4; i++) {
        g.drawRoundedRectangle(visualizerBounds.expanded((float)i * glowSpread), cornerSize, lineThickness);
    }

    juce::ColourGradient gradient(Colors::AudioVisualizer::visualizerBG.brighter(0.1f),
        visualizerBounds.getX(), visualizerBounds.getY(),
        Colors::AudioVisualizer::visualizerBG.darker(0.2f),
        visualizerBounds.getX(), visualizerBounds.getBottom(), false
    );

    g.setGradientFill(gradient);
    g.fillRoundedRectangle(visualizerBounds, cornerSize);
    g.drawRoundedRectangle(visualizerBounds, cornerSize, lineThickness);

    // Prevent waveform from being drawn past the bounds
    g.saveState();
    juce::Path clipPath;
    clipPath.addRoundedRectangle(visualizerBounds, cornerSize);
    g.reduceClipRegion(clipPath);

    AudioVisualiserComponent::paint(g);

    g.restoreState();
}

void Visualiser::resized()
{

}
