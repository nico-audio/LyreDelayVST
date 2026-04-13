/*
  ==============================================================================

    Parameters.h
    Created: 9 Sep 2025 12:38:54pm
    Author:  Nico V.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ParameterHelper.h"

class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void update() noexcept;
    void smoothen() noexcept;

    float gain = 0.0f;
    float delayTime = 0.0f;
    float mix = 1.0f;
    float feedback = 0.0f;
    float panL = 0.0f;
    float panR = 1.0f;
    float lowCut = 20.0f;
    float highCut = 20000.0f;
    float grainSize = 1.0f;
    float pitch = 0.0f;
    float density = 0.0f;
    float texture = 0.0f;
    int delayNote = 0;
    bool tempoSync = false;
    bool bypassed = false;
    bool granularisActive = false;
    bool randomIsActive = false;
   
    juce::AudioParameterBool* tempoSyncParam;
    juce::AudioParameterBool* bypassParam;
    juce::AudioParameterBool* granularToggleParam;
    juce::AudioParameterBool* randomPushParam;
    
private:
    // Gain
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;

    // Delay line
    juce::AudioParameterFloat* delayTimeParam;
    float targetDelayTime = 0.0f;
    float filterCoefficient = 0.0f;

    // Mix
    juce::AudioParameterFloat* mixParam;
    juce::LinearSmoothedValue<float> mixSmoother;

    // Feedback
    juce::AudioParameterFloat* feedbackParam;
    juce::LinearSmoothedValue<float> feedbackSmoother;

    // Stereo Width
    juce::AudioParameterFloat* stereoParam;
    juce::LinearSmoothedValue<float> stereoSmoother;

    // SVF Filter - high and low cut
    juce::AudioParameterFloat* lowCutParam;
    juce::LinearSmoothedValue<float> lowCutSmoother;

    juce::AudioParameterFloat* highCutParam;
    juce::LinearSmoothedValue<float> highCutSmoother;

    // Tempo sync
    juce::AudioParameterChoice* delayNoteParam;

    // Grain size
    juce::AudioParameterFloat* sizeParam;
    juce::LinearSmoothedValue<float> sizeSmoother;

    // Grain pitch
    juce::AudioParameterFloat* pitchParam;
    juce::LinearSmoothedValue<float> pitchSmoother;

    // Grain density
    juce::AudioParameterFloat* densityParam;
    juce::LinearSmoothedValue<float> densitySmoother;

    // Texture
    juce::AudioParameterFloat* textureParam;
    juce::LinearSmoothedValue<float> textureSmoother;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)
};