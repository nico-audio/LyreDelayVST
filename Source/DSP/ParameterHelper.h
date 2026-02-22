/*
  ==============================================================================

    ParameterHelper.h
    Created: 20 Feb 2026 10:49:59am
    Author:  Nico V.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Params 
{
    static const juce::ParameterID gainParamID{ "gain", 1 };
    static const juce::ParameterID delayTimeParamID{ "delayTime", 1 };
    static const juce::ParameterID mixParamID{ "mix", 1 };
    static const juce::ParameterID feedbackParamID{ "feedback", 1 };
    static const juce::ParameterID stereoParamID{ "stereoWidth", 1 };
    static const juce::ParameterID lowCutParamID{ "lowCut", 1 };
    static const juce::ParameterID highCutParamID{ "highCut", 1 };
    static const juce::ParameterID tempoSyncParamID{ "tempoSync", 1 };
    static const juce::ParameterID delayNoteParamID{ "delayNote", 1 };
    static const juce::ParameterID bypassParamID{ "bypass", 1 };
    static const juce::ParameterID granularToggleParamID{ "granularToggle", 1 };
    static const juce::ParameterID grainSizeParamID{ "grainSize", 1 };
    static const juce::ParameterID grainPitchParamID{ "grainPitch", 1 };
    static const juce::ParameterID grainDensityParamID{ "grainDensity", 1 };

    namespace ParameterNames
    {
        static constexpr auto gain = "Gain";
        static constexpr auto delaytime = "Delay time";
        static constexpr auto mix = "Dry/Wet";
        static constexpr auto feedback = "Feedback";
        static constexpr auto stereo = "Stereo Width";
        static constexpr auto lowCut = "Low Cut";
        static constexpr auto highCut = "High Cut";
        static constexpr auto size = "Size";
        static constexpr auto pitch = "Pitch";
        static constexpr auto density = "Density";
        static constexpr auto tempoSync = "Tempo Sync";
        static constexpr auto delayNote = "Delay Note";
        static constexpr auto bypass = "Bypass";
        static constexpr auto granular = "Granular";
    }

    namespace Range
    {
        static constexpr float minGain{ -12.0f };
        static constexpr float maxGain{ 12.0f };
        
        static constexpr float minDelayTime{ 5.0f };
        static constexpr float maxDelayTime{ 5000.0f };
        static constexpr float delayTimeStepSize{ 0.001f };
        static constexpr float delayTimeSkew{ 0.25f };

        static constexpr float minMix{ 0.0f };
        static constexpr float maxMix{ 100.0f };
        static constexpr float mixStepSize{ 1.0f };

        static constexpr float minFeedback{ -100.0f };
        static constexpr float maxFeedback{ 100.0f };
        static constexpr float feedbackStepSize{ 1.0f };

        static constexpr float minStereoWidth{ -100.0f };
        static constexpr float maxStereoWidth{ 100.0f };
        static constexpr float stereoStepSize{ 1.0f };

        static constexpr float minFilterCutoff{ 20.0f };
        static constexpr float maxFilterCutoff{ 20000.0f };
        static constexpr float filterStepSize{ 1.0f };
        static constexpr float filterSkewFactor{ 0.3f };

        static constexpr float minGrainSize{ 1.0f };
        static constexpr float maxGrainSize{ 100.0f };
        static constexpr float grainStepSize{ 1.0f };

        static constexpr float minPitch{ -24.0f };
        static constexpr float maxPitch{ 24.0f };
        static constexpr float pitchStepSize{ 0.01f };

        static constexpr float minDensity{ 0.0f };
        static constexpr float maxDensity{ 100.0f };
        static constexpr float densityStepSize{ 1.0f };

        static const juce::NormalisableRange<float> gainRange{ minGain, maxGain };
        static const juce::NormalisableRange<float> delayTimeRange{ minDelayTime, maxDelayTime, delayTimeStepSize, delayTimeSkew };
        static const juce::NormalisableRange<float> mixRange{ minMix, maxMix, mixStepSize };
        static const juce::NormalisableRange<float> feedbackRange{ minFeedback, maxFeedback, feedbackStepSize };
        static const juce::NormalisableRange<float> stereoRange{ minStereoWidth, maxStereoWidth , stereoStepSize };
        static const juce::NormalisableRange<float> lowCutRange{ minFilterCutoff, maxFilterCutoff, filterStepSize, filterSkewFactor };
        static const juce::NormalisableRange<float> highCutRange{ minFilterCutoff, maxFilterCutoff, filterStepSize, filterSkewFactor };
        static const juce::NormalisableRange<float> grainSizeRange { minGrainSize, maxGrainSize, grainStepSize };
        static const juce::NormalisableRange<float> pitchRange { minPitch, maxPitch, pitchStepSize };
        static const juce::NormalisableRange<float> densityRange{ minDensity, maxDensity, densityStepSize };
    }

    namespace Defaults 
    {
        static constexpr float defaultGain{ 0.0f };
        static constexpr float defaultDelayTime{ 100.0f };
        static constexpr float defaultMix{ 100.0f };
        static constexpr float defaultFeedback{ 0.0f };
        static constexpr float defaultStereo{ 0.0f };
        static constexpr float defaultHighCutoff{ 20000.0f };
        static constexpr float defaultLowCutoff{ 20.0f };
        static constexpr float defaultSize{ 1.0f };
        static constexpr float defaultPitch{ 0.0f };
        static constexpr float defaultDensity{ 0.0f };
    }
}