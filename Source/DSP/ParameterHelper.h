/*
  ==============================================================================

    ParameterHelper.h
    Created: 20 Feb 2026 10:49:59am
    Author:  Nico V.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Params {
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

    namespace ParameterNames{
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
}