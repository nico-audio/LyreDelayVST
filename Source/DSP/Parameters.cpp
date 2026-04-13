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
    castParameter(apvts, Params::textureParamID, textureParam);
    castParameter(apvts, Params::randomPushParamID, randomPushParam);
}                        

// Plugin parameters
juce::AudioProcessorValueTreeState::ParameterLayout
Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(Params::makeGainParam());
    layout.add(Params::makeDelayTimeParam());
    layout.add(Params::makeMixParam());
    layout.add(Params::makeFeedbackParam());
    layout.add(Params::makeStereoParam());
    layout.add(Params::makeLowCutParam());
    layout.add(Params::makeHighCutParam());
    layout.add(Params::makeSizeParam());
    layout.add(Params::makePitchParam());
    layout.add(Params::makeDensityParam());
    layout.add(Params::makeTextureParam());

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
    
    layout.add(std::make_unique<juce::AudioParameterBool>(Params::randomPushParamID, Params::ParameterNames::randomize, false));

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

    textureSmoother.reset(sampleRate, duration);
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

    texture = 0.0f;
    textureSmoother.setCurrentAndTargetValue(densityParam->get() * 0.01f);
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

    textureSmoother.setTargetValue(textureParam->get());

    delayNote = delayNoteParam->getIndex();
    tempoSync = tempoSyncParam->get();

    bypassed = bypassParam->get();

    granularisActive = granularToggleParam->get();

    randomIsActive = randomToggleParam->get();
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
    texture = textureSmoother.getNextValue();
}
