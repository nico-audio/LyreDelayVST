/*
  ==============================================================================

    Parameters.h
    Created: 9 Sep 2025 12:38:54pm
    Author:  Nico V.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

const juce::ParameterID gainParamID{ "gain", 1 };
const juce::ParameterID delayTimeParamID{ "delayTime", 1 };
const juce::ParameterID mixParamID{ "mix", 1 };
const juce::ParameterID feedbackParamID{ "feedback", 1 };
const juce::ParameterID stereoParamID{ "stereoWidth", 1 };
const juce::ParameterID lowCutParamID{ "lowCut", 1 };
const juce::ParameterID highCutParamID{ "highCut", 1 };
const juce::ParameterID tempoSyncParamID{ "tempoSync", 1 };
const juce::ParameterID delayNoteParamID{ "delayNote", 1 };

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
    int delayNote = 0;
    bool tempoSync = false;
   
    static constexpr float minDelayTime { 5.0f };
    static constexpr float maxDelayTime { 5000.0f };
    static constexpr float delayTimeStepSize { 0.001f };
    static constexpr float delayTimeSkew { 0.25f };

    static constexpr float minMix{ 0.0f };
    static constexpr float maxMix{ 100.0f };
    static constexpr float mixStepSize{ 1.0f };
    static constexpr float defaultMix{ 100.0f };

    static constexpr float minFilterCutoff { 20.0f };
    static constexpr float maxFilterCutoff { 20000.0f };
    static constexpr float filterStepSize { 1.0f };
    static constexpr float filterSkewFactor { 0.3f };
    static constexpr float defaultHighCutoff { 20000.0f };
    static constexpr float defaultLowCutoff{ 20.0f };

    juce::AudioParameterBool* tempoSyncParam;
    
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)
};