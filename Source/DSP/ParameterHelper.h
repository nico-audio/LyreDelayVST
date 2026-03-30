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
    static const juce::ParameterID textureParamID{ "texture", 1 };

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
        static constexpr auto texture = "Texture";
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
        static constexpr float maxGrainSize{ 500.0f };
        static constexpr float grainStepSize{ 1.0f };

        static constexpr float minPitch{ -24.0f };
        static constexpr float maxPitch{ 24.0f };
        static constexpr float pitchStepSize{ 0.01f };

        static constexpr float minDensity{ 1.0f };
        static constexpr float maxDensity{ 200.0f };
        static constexpr float densityStepSize{ 1.0f };

        static constexpr float minTexture{ 0.0f };
        static constexpr float maxTexture{ 1.0f };
        static constexpr float textureStepSize{ 0.01f };

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
        static const juce::NormalisableRange<float> textureRange{ minTexture, maxTexture, textureStepSize };
    }

    namespace Defaults 
    {
        static constexpr float defaultGain{ 0.0f };
        static constexpr float defaultDelayTime{ 100.0f };
        static constexpr float defaultMix{ 50.0f };
        static constexpr float defaultFeedback{ 0.0f };
        static constexpr float defaultStereo{ 0.0f };
        static constexpr float defaultHighCutoff{ 20000.0f };
        static constexpr float defaultLowCutoff{ 20.0f };
        static constexpr float defaultSize{ 1.0f };
        static constexpr float defaultPitch{ 0.0f };
        static constexpr float defaultDensity{ 0.0f };
        static constexpr float defaultTexture{ 0.0f };
    }

    // Parameter Units conversion

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
            if (text.endsWithIgnoreCase("s") || value < Range::minDelayTime) {
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

    static juce::String stringFromTexture(float value, int)
    {
        if (value < 0.2f) return juce::String("Tight");
        if (value < 0.4f) return juce::String("Soft");
        if (value < 0.6f) return juce::String("Smooth");
        if (value < 0.8f) return juce::String("Diffuse");
        
        return juce::String("Cloudy");
    }

    static float hzFromString(const juce::String& str)
    {
        float value = str.getFloatValue();
        if (value < 20.0f) {
            return value * 1000.0f;
        }
        return value;
    }

    static float textureFromString(const juce::String& str)
    {
        if (str == "Tight") return 0.1f;
        if (str == "Soft") return 0.3f;
        if (str == "Smooth") return 0.5f;
        if (str == "Diffuse") return 0.7f;
        
        return 0.9f;
    }

    // Make parameter functions

    inline auto makeGainParam() 
    {
        return std::make_unique<juce::AudioParameterFloat>(
            gainParamID, ParameterNames::gain, Range::gainRange, Defaults::defaultGain,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)
        );
    }

    inline auto makeDelayTimeParam()
    {
        return std::make_unique<juce::AudioParameterFloat>(
            delayTimeParamID, ParameterNames::delaytime , Range::delayTimeRange, Defaults::defaultDelayTime,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromMilliseconds)
                                                 .withValueFromStringFunction(millisecondsFromString)
        );
    }

    inline auto makeMixParam()
    {
        return std::make_unique<juce::AudioParameterFloat>(
            mixParamID, ParameterNames::mix, Range::mixRange, Defaults::defaultMix,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
        );
    }

    inline auto makeFeedbackParam()
    {
        return std::make_unique<juce::AudioParameterFloat>(
            feedbackParamID, ParameterNames::feedback, Range::feedbackRange, Defaults::defaultFeedback,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
        );
    }

    inline auto makeStereoParam()
    {
        return std::make_unique<juce::AudioParameterFloat>(
            stereoParamID, ParameterNames::stereo, Range::stereoRange, Defaults::defaultStereo,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
        );
    }

    inline auto makeLowCutParam()
    {
        return std::make_unique<juce::AudioParameterFloat>(
            lowCutParamID, ParameterNames::lowCut, Range::lowCutRange, Defaults::defaultLowCutoff,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromHz)
                                                 .withValueFromStringFunction(hzFromString)
        );
    }

    inline auto makeHighCutParam()
    {
        return std::make_unique<juce::AudioParameterFloat>(
            highCutParamID, ParameterNames::highCut, Range::highCutRange, Defaults::defaultHighCutoff,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromHz)
                                                 .withValueFromStringFunction(hzFromString)
        );
    }

    inline auto makeSizeParam()
    {
        return std::make_unique<juce::AudioParameterFloat>(
            grainSizeParamID, ParameterNames::size, Range::grainSizeRange, Defaults::defaultSize,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromMilliseconds)
                                                 .withValueFromStringFunction(millisecondsFromString)
        );
    }

    inline auto makePitchParam()
    {
        return std::make_unique<juce::AudioParameterFloat>(
            grainPitchParamID, ParameterNames::pitch, Range::pitchRange, Defaults::defaultPitch,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromSemitone)
        );
    }

    inline auto makeDensityParam()
    {
        return std::make_unique<juce::AudioParameterFloat>(
            grainDensityParamID, ParameterNames::density, Range::densityRange, Defaults::defaultDensity,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromHz)
                                                 .withValueFromStringFunction(hzFromString)
        );
    }

    inline auto makeTextureParam()
    {
        return std::make_unique<juce::AudioParameterFloat>(
            textureParamID, ParameterNames::texture, Range::textureRange, Defaults::defaultTexture,
            juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromTexture)
                                                 .withValueFromStringFunction(textureFromString)
        );
    }
}

