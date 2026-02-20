/*
  ==============================================================================

    Parameters.cpp
    Created: 9 Sep 2025 12:38:54pm
    Author:  Nico V.

  ==============================================================================
*/

#include "Parameters.h"
#include "../DSP.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
    const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);
    // parameter does not exist or wrong type
}

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 1) + " dB";
}

static juce::String stringFromMilliseconds(float value, int)
{
    if (value < 10.0f) {
        return juce::String(value, 2) + " ms";
    }
    else if (value < 100.0f) {
        return juce::String(value, 1) + " ms";
    }
    else if (value < 1000.0f) {
        return juce::String(int(value)) + " ms";
    }
    else {
        return juce::String(value * 0.001f, 2) + " s";
    }
}

static float millisecondsFromString(const juce::String& text)
{
    float value = text.getFloatValue();

    if (!text.endsWithIgnoreCase("ms")) {
        if (text.endsWithIgnoreCase("s") || value < Parameters::minDelayTime) {
            return value * 1000.0f;
        }
    }
    return value;
}

static juce::String stringFromPercent(float value, int)
{
    return juce::String(int(value)) + " %";
}

static juce::String stringFromHz(float value, int)
{
    if (value < 1000.0f) {
        return juce::String(int(value)) + " Hz";
    }
    else if (value < 10000.0f) {
        return juce::String(value / 1000.0f, 2) + " k";
    }
    else {
        return juce::String(value / 1000.0f, 1) + " k";
    }
}

static juce::String stringFromSemitone(float value, int)
{
    return juce::String(int(value)) + " st";
}

static float hzFromString(const juce::String& str)
{
    float value = str.getFloatValue();
    if (value < 20.0f) {
        return value * 1000.0f;
    }
    return value;
}

// Constructor
Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, Params::gainParamID, gainParam);
    castParameter(apvts, Params::delayTimeParamID, delayTimeParam);
    castParameter(apvts, Params::mixParamID, mixParam);
    castParameter(apvts, Params::feedbackParamID, feedbackParam);
    castParameter(apvts, Params::stereoParamID, stereoParam);
    castParameter(apvts, Params::lowCutParamID, lowCutParam);
    castParameter(apvts, Params::highCutParamID, highCutParam);
    castParameter(apvts, Params::tempoSyncParamID, tempoSyncParam);
    castParameter(apvts, Params::delayNoteParamID, delayNoteParam);
    castParameter(apvts, Params::bypassParamID, bypassParam);
    castParameter(apvts, Params::granularToggleParamID, granularToggleParam);
    castParameter(apvts, Params::grainSizeParamID, sizeParam);
    castParameter(apvts, Params::grainPitchParamID, pitchParam);
    castParameter(apvts, Params::grainDensityParamID, densityParam);
}                        

// Plugin parameters
juce::AudioProcessorValueTreeState::ParameterLayout
Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::gainParamID,
        Params::ParameterNames::gain,
        juce::NormalisableRange<float> { -12.0f, 12.0f },
        0.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::delayTimeParamID,
        Params::ParameterNames::delaytime,
        juce::NormalisableRange<float> { minDelayTime, maxDelayTime, delayTimeStepSize, delayTimeSkew },
        100.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromMilliseconds)
                                             .withValueFromStringFunction(millisecondsFromString)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::mixParamID,
        Params::ParameterNames::mix,
        juce::NormalisableRange<float> { minMix, maxMix, mixStepSize },
        defaultMix,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::feedbackParamID,
        Params::ParameterNames::feedback,
        juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::stereoParamID,
        Params::ParameterNames::stereo,
        juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::lowCutParamID,
        Params::ParameterNames::lowCut,
        juce::NormalisableRange<float> { minFilterCutoff, maxFilterCutoff, filterStepSize, filterSkewFactor },
        defaultLowCutoff,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromHz)
                                             .withValueFromStringFunction(hzFromString)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::highCutParamID,
        Params::ParameterNames::highCut,
        juce::NormalisableRange<float> { minFilterCutoff, maxFilterCutoff, filterStepSize, filterSkewFactor },
        defaultHighCutoff,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromHz)
                                             .withValueFromStringFunction(hzFromString)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::grainSizeParamID,
        Params::ParameterNames::size,
        juce::NormalisableRange<float> { minGrainSize, maxGrainSize, grainStepSize },
        defaultSize,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromMilliseconds)
                                             .withValueFromStringFunction(millisecondsFromString)
    ));

    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::grainPitchParamID,
        Params::ParameterNames::pitch,
        juce::NormalisableRange<float> { minPitch , maxPitch , pitchStepSize },
        defaultPitch,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromSemitone)
    ));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Params::grainDensityParamID,
        Params::ParameterNames::density,
        juce::NormalisableRange<float> { minDensity, maxDensity, densityStepSize },
        defaultDensity,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromHz)
                                             .withValueFromStringFunction(hzFromString)
    ));

    layout.add(std::make_unique<juce::AudioParameterBool>(Params::tempoSyncParamID, Params::ParameterNames::tempoSync, false));

    juce::StringArray noteLengths = {
        "1/32",
        "1/16 trip",
        "1/32 dot",
        "1/16",
        "1/8 trip",
        "1/16 dot",
        "1/8",
        "1/4 trip",
        "1/8 dot",
        "1/4",
        "1/2 trip",
        "1/4 dot",
        "1/2",
        "1/1 trip",
        "1/2 dot",
        "1/1",
    };

    layout.add(std::make_unique<juce::AudioParameterChoice>(Params::delayNoteParamID, Params::ParameterNames::delayNote, noteLengths, 9));
    
    layout.add(std::make_unique<juce::AudioParameterBool>(Params::bypassParamID, Params::ParameterNames::bypass, false));

    layout.add(std::make_unique<juce::AudioParameterBool>(Params::granularToggleParamID, Params::ParameterNames::granular, false));

    return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);

    // Used for parameter smoothing
    filterCoefficient = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate)));

    mixSmoother.reset(sampleRate, duration);
    
    feedbackSmoother.reset(sampleRate, duration);

    stereoSmoother.reset(sampleRate, duration);

    lowCutSmoother.reset(sampleRate, duration);
    highCutSmoother.reset(sampleRate, duration);

    sizeSmoother.reset(sampleRate, duration);

    pitchSmoother.reset(sampleRate, duration);

    densitySmoother.reset(sampleRate, duration);
}

void Parameters::reset() noexcept
{
    gain = 0.0f;
    gainSmoother.setCurrentAndTargetValue(
        juce::Decibels::decibelsToGain(gainParam->get()));
    
    delayTime = 0.0f;

    mix = 1.0f;
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);

    feedback = 0.0f;
    feedbackSmoother.setCurrentAndTargetValue(feedbackParam->get() * 0.01f);

    panL = 0.0f;
    panR = 1.0f;
    stereoSmoother.setCurrentAndTargetValue(stereoParam->get() * 0.01f);

    lowCut = 20.0f;
    lowCutSmoother.setCurrentAndTargetValue(lowCutParam->get());

    highCut = 20000.0f;
    highCutSmoother.setCurrentAndTargetValue(highCutParam->get());

    grainSize = 1.0f;
    sizeSmoother.setCurrentAndTargetValue(sizeParam->get() * 0.01f);

    pitch = 0.0f;
    pitchSmoother.setCurrentAndTargetValue(pitchParam->get() * 0.01f);

    density = 0.0f;
    densitySmoother.setCurrentAndTargetValue(densityParam->get() * 0.01f);
}

void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    targetDelayTime = delayTimeParam->get();
    if (delayTime == 0.0f) {
        delayTime == targetDelayTime;
    }
    
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    feedbackSmoother.setTargetValue(feedbackParam->get() * 0.01f);
    stereoSmoother.setTargetValue(stereoParam->get() * 0.01f);

    lowCutSmoother.setTargetValue(lowCutParam->get());
    highCutSmoother.setTargetValue(highCutParam->get());

    sizeSmoother.setTargetValue(sizeParam->get());

    pitchSmoother.setTargetValue(pitchParam->get());

    densitySmoother.setTargetValue(densityParam->get());

    delayNote = delayNoteParam->getIndex();
    tempoSync = tempoSyncParam->get();

    bypassed = bypassParam->get();

    granularisActive = granularToggleParam->get();
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    delayTime = targetDelayTime;
    mix = mixSmoother.getNextValue();
    feedback = feedbackSmoother.getNextValue();
    
    panningEqualPower(stereoSmoother.getNextValue(), panL, panR);

    lowCut = lowCutSmoother.getNextValue();
    highCut = highCutSmoother.getNextValue();

    grainSize = sizeSmoother.getNextValue();
    pitch = pitchSmoother.getNextValue();
    density = densitySmoother.getNextValue();
}
