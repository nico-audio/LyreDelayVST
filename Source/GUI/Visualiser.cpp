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
    auto visualizerGlow = visualizerBounds;

    g.setColour(Colors::AudioVisualizer::visualizerWave.withAlpha(0.15f));

    for (int i = 0; i < 4; i++) {
        g.fillRoundedRectangle(visualizerGlow.expanded(i * 1.5f), 10.0f);
    }

    juce::ColourGradient gradient(Colors::AudioVisualizer::visualizerBG.brighter(0.1f),
        visualizerBounds.getX(), visualizerBounds.getY(),
        Colors::AudioVisualizer::visualizerBG.darker(0.2f),
        visualizerBounds.getX(), visualizerBounds.getBottom(), false
    );

    g.setGradientFill(gradient);
    g.fillRoundedRectangle(visualizerBounds, 6.0f);
    g.drawRoundedRectangle(visualizerBounds, 6.0f, 1.0f);

    AudioVisualiserComponent::paint(g);
}

void Visualiser::resized()
{

}
