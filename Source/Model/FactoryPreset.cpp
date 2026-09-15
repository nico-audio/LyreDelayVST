/*
  ==============================================================================

    FactoryPreset.cpp
    Created: 23 Jul 2026 11:08:13am
    Author:  Nico V.

  ==============================================================================
*/

#include "FactoryPreset.h"

namespace
{
    const FactoryPreset factoryPresets[]
    {
        { "Anime", BinaryData::Anime_preset, BinaryData::Anime_presetSize },
        { "Answers", BinaryData::Answers_preset, BinaryData::Answers_presetSize },
        { "Bouncy Eight", BinaryData::Bouncy_Eight_preset , BinaryData::Bouncy_Eight_presetSize },
        { "Brick", BinaryData::Brick_preset , BinaryData::Brick_presetSize },
        { "Chills", BinaryData::Chills_preset, BinaryData::Chills_presetSize },
        { "Crystals", BinaryData::Crystals_preset, BinaryData::Crystals_presetSize },
        { "Digital", BinaryData::Digital_preset, BinaryData::Digital_presetSize },
        { "Dissipation", BinaryData::Dissipation_preset, BinaryData::Dissipation_presetSize },
        { "Dreamy", BinaryData::Dreamy_preset, BinaryData::Dreamy_presetSize },
        { "Enchantment", BinaryData::Enchantment_preset, BinaryData::Enchantment_presetSize },
        { "Haunted", BinaryData::Haunted_preset, BinaryData::Haunted_presetSize },
        { "Microns", BinaryData::Microns_preset, BinaryData::Microns_presetSize },
        { "Numb", BinaryData::Numb_preset, BinaryData::Numb_presetSize },
        { "Quarter Pong", BinaryData::Quarter_Pong_preset, BinaryData::Quarter_Pong_presetSize },
        { "Razor cut", BinaryData::Razor_cut_preset, BinaryData::Razor_cut_presetSize },
        { "Robotic Monster", BinaryData::Robotic_monster_preset, BinaryData::Robotic_monster_presetSize },
        { "Sixteen Flutter", BinaryData::Sixteen_flutter_preset, BinaryData::Sixteen_flutter_presetSize },
        { "Slapback", BinaryData::Slapback_preset, BinaryData::Slapback_presetSize },
        { "Spirals", BinaryData::Spirals_preset, BinaryData::Spirals_presetSize },
        { "Static", BinaryData::Static_preset, BinaryData::Static_presetSize },
        { "Swing", BinaryData::Swing_preset, BinaryData::Swing_presetSize },
        { "Tails", BinaryData::Tails_preset, BinaryData::Tails_presetSize },
        { "Texturize", BinaryData::Texturize_preset, BinaryData::Texturize_presetSize },
        { "Twilight Clouds", BinaryData::Twilight_clouds_preset, BinaryData::Twilight_clouds_presetSize },
        { "Twisted sister", BinaryData::Twisted_sister_preset, BinaryData::Twisted_sister_presetSize },
        { "Underground", BinaryData::Underground_preset, BinaryData::Underground_presetSize },
        { "Waves", BinaryData::Waves_preset, BinaryData::Waves_presetSize }
    };

    constexpr int numFactoryPresets = std::size(factoryPresets);

}
const FactoryPreset* findFactoryPreset(const juce::String& name)
{
    for (const auto& preset : factoryPresets) {
        if (name == preset.name) {
            return &preset;
        }
    }
    return nullptr;
}

juce::StringArray getFactoryPresetNames()
{
    juce::StringArray names;

    for (const auto& preset : factoryPresets) {
        names.add(preset.name);
    }
    
    return names;
}
