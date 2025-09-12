/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GDelayAudioProcessor::GDelayAudioProcessor():
    AudioProcessor(
        BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    ),
    params(apvts)
{
    // do nothing
}

GDelayAudioProcessor::~GDelayAudioProcessor()
{
}

//==============================================================================
const juce::String GDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void GDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params.prepareToPlay(sampleRate);
    params.reset();

    juce::dsp::ProcessSpec specifications;
    specifications.sampleRate = sampleRate;
    specifications.maximumBlockSize = juce::uint32(samplesPerBlock);
    specifications.numChannels = 2;
    delayLine.prepare(specifications);

    double numSamples = Parameters::maxDelayTime / 1000.0 * sampleRate;
    int maxDelayInSamples = int(std::ceil(numSamples));
    delayLine.setMaximumDelayInSamples(maxDelayInSamples);
    delayLine.reset();

    // DBG(maxDelayInSamples);
}

void GDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#endif

void GDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Update parameters
    params.update();

    // Set delay length
    delayLine.setDelay(48000.0f);

    // Get write pointers for left and right channels
    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);

    // Apply smoothed gain per sample
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        params.smoothen();

        float dryL = channelDataL[sample];
        float dryR = channelDataR[sample];

        delayLine.pushSample(0, dryL);
        delayLine.pushSample(1, dryR);

        float wetL = delayLine.popSample(0);
        float wetR = delayLine.popSample(1);

        channelDataL[sample] = (dryL + wetL) * params.gain;
        channelDataR[sample] = (dryR + wetR) * params.gain;
    }
}


    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    */

//==============================================================================
bool GDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GDelayAudioProcessor::createEditor()
{
    return new GDelayAudioProcessorEditor (*this);
}

//==============================================================================
void GDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void GDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GDelayAudioProcessor();
}
