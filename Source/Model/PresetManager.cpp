/*
  ==============================================================================

    PresetManager.cpp
    Created: 9 Jun 2026 11:20:20am
    Author:  Nico V.

  ==============================================================================
*/

#include "PresetManager.h"

const juce::File PresetManager::defaultDirectory{ juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory)
                                                                                  .getChildFile("LyreDelay").getChildFile(ProjectInfo::projectName)
};
const juce::String PresetManager::extension{ "preset" };
const juce::String PresetManager::presetNameProperty{ "presetName" };

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts) : valueTreeState(apvts)
{
    if (!defaultDirectory.exists()) {
        const auto result = defaultDirectory.createDirectory();
        if (result.failed()) {
            DBG("Failed to create preset directory" + result.getErrorMessage());
            jassertfalse;
        }
    }
    
    valueTreeState.state.addListener(this);
    currentPreset.referTo(valueTreeState.state.getPropertyAsValue(presetNameProperty, nullptr));
}

void PresetManager::savePreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) {
        return;
    }

    currentPreset.setValue(presetName);
    //DBG(valueTreeState.state.toXmlString())
    valueTreeState.state.setProperty("version", ProjectInfo::versionString, nullptr);

    const auto xml = valueTreeState.copyState().createXml();
    const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
    if (!xml->writeTo(presetFile)) {
        DBG("Could not create preset file: " + presetFile.getFullPathName());
    }
}

void PresetManager::deletePreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) {
        return;
    }

    const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
    if (!presetFile.existsAsFile()) {
        DBG("Preset file " + presetFile.getFullPathName() + " does not exist");
        jassertfalse;
        return;
    }
    
    if (!presetFile.deleteFile()) {
        DBG("Preset file " + presetFile.getFullPathName() + " could not be deleted");
        jassertfalse;
        return;
    }

    currentPreset.setValue("");
}

void PresetManager::loadPreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) {
        return;
    }

    const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);

    if (!presetFile.existsAsFile()){
        DBG("Preset file " + presetFile.getFullPathName() + " does not exist");
        jassertfalse;
        return;
    }

    juce::XmlDocument xmlDocument{ presetFile };
    const auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDocument.getDocumentElement());
    valueTreeState.replaceState(valueTreeToLoad);
    currentPreset.setValue(presetName);
}

int PresetManager::loadNextPreset()
{
    const auto allPresets = getAllPresets();

    if (allPresets.isEmpty()) {
        return -1;
    }

    const auto currentIndex = allPresets.indexOf(currentPreset.toString());
    const auto nextIndex = currentIndex + 1 > (allPresets.size() - 1) ? 0 : currentIndex + 1;
    loadPreset(allPresets.getReference(nextIndex));
    
    return nextIndex;
}

int PresetManager::loadPreviousPreset()
{
    const auto allPresets = getAllPresets();

    if (allPresets.isEmpty()) {
        return -1;
    }

    const auto currentIndex = allPresets.indexOf(currentPreset.toString());
    const auto previousIndex = currentIndex - 1 < 0 ? allPresets.size() - 1 : currentIndex - 1;
    loadPreset(allPresets.getReference(previousIndex));
    return previousIndex;
}

juce::StringArray PresetManager::getAllPresets() const
{
    juce::StringArray presets;
    const auto fileArray = defaultDirectory.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false, "*." + extension);

    for (const auto& file : fileArray) {
        presets.add(file.getFileNameWithoutExtension());
    }

    return presets;
}

juce::String PresetManager::getCurrentPreset() const
{
    return currentPreset.toString();
}

void PresetManager::valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged)
{
    //treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr);

    currentPreset.referTo(treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty,nullptr));
}
