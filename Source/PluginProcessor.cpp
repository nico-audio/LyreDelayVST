/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <cmath>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ProtectYourEars.h"
#include "DSP/Grain.h"


GDelayAudioProcessor::GDelayAudioProcessor():
    AudioProcessor(
        BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    ),
    params(apvts),
    waveViewer(2)
{
    lowCutFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    highCutFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);

    waveViewer.setRepaintRate(30);
    waveViewer.setBufferSize(256);
}

GDelayAudioProcessor::~GDelayAudioProcessor()
{
}

void GDelayAudioProcessor::randomizeParams()
{
    if (params.granularisActive == true) {
        params.randomizeGranularParams();
    }
}

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

void GDelayAudioProcessor::setCurrentProgram (int)
{
}

const juce::String GDelayAudioProcessor::getProgramName (int)
{
    return {};
}

void GDelayAudioProcessor::changeProgramName (int index, const juce::String&)
{
}

void GDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params.prepareToPlay(sampleRate);
    params.reset();

    juce::dsp::ProcessSpec specifications;
    specifications.sampleRate = sampleRate;
    specifications.maximumBlockSize = juce::uint32(samplesPerBlock);
    specifications.numChannels = 2;


    double numSamples = Params::Range::maxDelayTime / 1000.0 * sampleRate;
    int maxDelayInSamples = int(std::ceil(numSamples));
    delayLineL.setMaximumDelayInSamples(maxDelayInSamples);
    delayLineR.setMaximumDelayInSamples(maxDelayInSamples);
    delayLineL.reset();
    delayLineR.reset();

    // DBG(maxDelayInSamples);

    // Clear out any old sample values from the feedback path
    feedbackL = 0.0f;
    feedbackR = 0.0f;

    lowCutFilter.prepare(specifications);
    lowCutFilter.reset();

    highCutFilter.prepare(specifications);
    highCutFilter.reset();

    lastLowCut = -1.0f;
    lastHighCut = -1.0f;

    tempo.reset();

    levelL.reset();
    levelR.reset();

    // Ducking
    delayInSamples = 0.0f;
    targetDelay = 0.0f;

    fade = 1.0f;
    fadeTarget = 1.0f;

    coefficient = 1.0f - std::exp(-1.0f / (0.05f * float(sampleRate)));

    wait = 0.0f;
    waitInc = 1.0f / (0.3f * float(sampleRate));  // 300 ms

    grEngine.prepare(sampleRate, maxDelayInSamples);
}

void GDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto mono = juce::AudioChannelSet::mono();
    const auto stereo = juce::AudioChannelSet::stereo();
    const auto mainIn = layouts.getMainInputChannelSet();
    const auto mainOut = layouts.getMainOutputChannelSet();

    // DBG ("isBusesLayoutSupported, in: " << mainIn.getDescription() << ", out: " << mainOut.getDescription());

    if (mainIn == mono && mainOut == mono) { return true; }
    if (mainIn == mono && mainOut == stereo) { return true; }
    if (mainIn == stereo && mainOut == stereo) { return true; }

    return false;
}
#endif

void GDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Update parameters
    params.update();

    //if (params.bypassed) { return; }

    // Get tempo from host to sync delay time
    tempo.update(getPlayHead());

    float syncedTime = float(tempo.getMillisecondsForNoteLength(params.delayNote));
    if (syncedTime > Params::Range::maxDelayTime) {
        syncedTime = Params::Range::maxDelayTime;
    }

    // Get sample rate
    float sampleRate = float(getSampleRate());

    /* Get read and write pointers for the input and output buffers. Handles mono-to-stereo processing by using the left channel's data
    for the right channel if the input is mono.*/

    auto mainInput = getBusBuffer(buffer, true, 0);
    auto mainInputChannels = mainInput.getNumChannels();
    auto isMainInputStereo = mainInputChannels > 1;
    const float* inputDataL = mainInput.getReadPointer(0);
    const float* inputDataR = mainInput.getReadPointer(isMainInputStereo ? 1 : 0);

    auto mainOutput = getBusBuffer(buffer, false, 0);
    auto mainOutputChannels = mainOutput.getNumChannels();
    auto isMainOutputStereo = mainOutputChannels > 1;
    float* outputDataL = mainOutput.getWritePointer(0);
    float* outputDataR = mainOutput.getWritePointer(isMainOutputStereo ? 1 : 0);

    // Level meter local variables
    float maxL = 0.0f;
    float maxR = 0.0f;
    
    // Apply smoothed gain per sample
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        params.smoothen();

        float delayTime = params.tempoSync ? syncedTime : params.delayTime;
        float newTargetDelay = millisecondsToSamples(delayTime, sampleRate);

        // fade if delay changes
        if (newTargetDelay != targetDelay) {
            targetDelay = newTargetDelay;
            if (delayInSamples == 0.0f) {
                delayInSamples = targetDelay;
            }
            else {
                wait = waitInc;     // start ducking wait counter
                fadeTarget = 0.0f;
            }
        }
   
        // Filter set cutoff - only calls setCutoffFrequency if values are changed
        if (params.lowCut != lastLowCut) {
            lowCutFilter.setCutoffFrequency(params.lowCut);
            lastLowCut = params.lowCut;
        }
        if (params.highCut != lastHighCut) {
            highCutFilter.setCutoffFrequency(params.highCut);
            lastHighCut = params.highCut;
        }

        // Read pointers
        float dryL = inputDataL[sample];
        float dryR = inputDataR[sample];

        // convert stereo to mono -  takes the average value of both channels
        float mono = (dryL + dryR) * 0.5f;

        // write mono signal into the delay line
        delayLineL.write(mono * params.panL + feedbackR);
        delayLineR.write(mono * params.panR + feedbackL);

        float wetL = delayLineL.read(delayInSamples);
        float wetR = delayLineR.read(delayInSamples);

        fade += (fadeTarget - fade) * coefficient;

        wetL *= fade;
        wetR *= fade;

        if (wait > 0.0f) {
            wait += waitInc;
            if (wait >= 1.0f) {
                delayInSamples = targetDelay;
                wait = 0.0f;
                fadeTarget = 1.0f;
            }
        }

        /*Multi-tap delay
        wetL += delayLine.popSample(0, delayInSamples * 2.0f, false) * 0.7f;
        wetR += delayLine.popSample(1, delayInSamples * 2.0f, false) * 0.7f;
        */

        feedbackL = wetL * params.feedback;
        feedbackL = lowCutFilter.processSample(0, feedbackL);
        feedbackL = highCutFilter.processSample(0, feedbackL);

        feedbackR = wetR * params.feedback;
        feedbackR = lowCutFilter.processSample(1, feedbackR);
        feedbackR = highCutFilter.processSample(1, feedbackR);
        
        grEngine.setParameters(params.density, params.grainSize, params.pitch, params.texture, params.granularisActive);

        float grainL = 0.0f;
        float grainR = 0.0f;

        grEngine.process(grainL, grainR, delayLineL, delayLineR);

        if (params.granularisActive) {
            wetL = grainL;
            wetR = grainR;
        }
       
        //float mixL = dryL + wetL * params.mix;
        //float mixR = dryR + wetR * params.mix;

        float mixL = dryL * (1.0f - params.mix) + wetL * params.mix;
        float mixR = dryR * (1.0f - params.mix) + wetR * params.mix;

        // Write pointers
        float outL = mixL * params.gain;
        float outR = mixR * params.gain;

        if (params.bypassed) {
            outL = dryL;
            outR = dryR;
        }

        outputDataL[sample] = outL;
        outputDataR[sample] = outR;

        // to lvl meter
        maxL = std::max(maxL, std::abs(outL));
        maxR = std::max(maxR, std::abs(outR));
    }

    // Store level measurements, only write max variables into level if the current value of level is smaller.
    levelL.updateIfGreater(maxL);
    levelR.updateIfGreater(maxR);
    
    // Push waveform to audio visualizer component
    waveViewer.pushBuffer(buffer);
   
    #if JUCE_DEBUG
        protectYourEars(buffer);
    #endif
   
}   

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

juce::AudioProcessorParameter* GDelayAudioProcessor::getBypassParameter() const
{
    return params.bypassParam;
}
