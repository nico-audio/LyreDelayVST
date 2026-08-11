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
        { "Bouncy Eight", BinaryData::Bouncy_Eight_preset , BinaryData::Bouncy_Eight_presetSize },
        { "Chills", BinaryData::Chills_preset, BinaryData::Chills_presetSize },
        { "Crystals", BinaryData::Crystals_preset, BinaryData::Crystals_presetSize },
        { "Digital", BinaryData::Digital_preset, BinaryData::Digital_presetSize },
        { "Dissipation", BinaryData::Dissipation_preset, BinaryData::Dissipation_presetSize },
        { "Dreamy", BinaryData::Dreamy_preset, BinaryData::Dreamy_presetSize },
        { "Drown", BinaryData::Drown_preset, BinaryData::Drown_presetSize },
        { "Enchantment", BinaryData::Enchantment_preset, BinaryData::Enchantment_presetSize },
        { "Extra gas", BinaryData::Extra_gas_preset, BinaryData::Extra_gas_presetSize },
        { "Microns", BinaryData::Microns_preset, BinaryData::Microns_presetSize },
        { "Quarter Pong", BinaryData::Quarter_Pong_preset, BinaryData::Quarter_Pong_presetSize },
        { "Razor cut", BinaryData::Razor_cut_preset, BinaryData::Razor_cut_presetSize },
        { "Robotic Monster", BinaryData::Robotic_monster_preset, BinaryData::Robotic_monster_presetSize },
        { "Sixteen Flutter", BinaryData::Sixteen_flutter_preset, BinaryData::Sixteen_flutter_presetSize },
        { "Slapback", BinaryData::Slapback_preset, BinaryData::Slapback_presetSize },
        { "Tails", BinaryData::Tails_preset, BinaryData::Tails_presetSize },
        { "Twilight Clouds", BinaryData::Twilight_clouds_preset, BinaryData::Twilight_clouds_presetSize },
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
