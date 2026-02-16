/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/Parameters.h"
#include "DSP/Tempo.h"
#include "DSP/DelayLine.h"
#include "Measurement.h"
#include "DSP/Grain.h"

class GDelayAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    GDelayAudioProcessor();
    ~GDelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorParameter* getBypassParameter() const override;
    
    juce::AudioProcessorValueTreeState apvts{
        *this, nullptr, "Parameters", Parameters::createParameterLayout()
    };

    juce::AudioVisualiserComponent waveViewer;

    static float millisecondsToSamples(float milliseconds, float sampleRate)
    {
        return milliseconds / 1000.0f * sampleRate;
    }

    Parameters params;

    Measurement levelL, levelR;

private:
    DelayLine delayLineL, delayLineR;

    float feedbackL = 0.0f;
    float feedbackR = 0.0f;

    juce::dsp::StateVariableTPTFilter<float> lowCutFilter;
    juce::dsp::StateVariableTPTFilter<float> highCutFilter;

    float lastLowCut = -1.0f;
    float lastHighCut = -1.0f;

    Tempo tempo;
    
    // Ducking
    float delayInSamples = 0.0f;
    float targetDelay = 0.0f;
    float fade = 0.0f;
    float fadeTarget = 0.0f;
    float coefficient = 0.0f;
    float wait = 0.0f;
    float waitInc = 0.0f;      

    static constexpr int maxGrains = 8;
    std::array<Grain, maxGrains> grainPool;
    static Grain* findAvailableGrain(std::array<Grain, maxGrains>& pool);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GDelayAudioProcessor)
};
