/*
  ==============================================================================
 * File: [Tempo.h]
 * Author:  [Nico V.]
 * Last updated:[03/11/2025]
 * Notes:
 * - reset: resets the internal state when audio playback starts or restarts.
 * - update: This function will be called from within processBlock to inspect 
 *           the current state of the host’s audio transport using the 
 *           juce::AudioPlayHead object.
 * - getMillisecondsForNoteLength: convert a give note length into a time in 
 *                                 milliseconds.
 * - getTempo: returns the value of the private member variable bpm
  =============================================================================
*/

#pragma once

#pragma once
#include <JuceHeader.h>

class Tempo
{
public:
    void reset() noexcept;
    void update(const juce::AudioPlayHead* playhead) noexcept;
    double getMillisecondsForNoteLength(int index) const noexcept;
    double getTempo() const noexcept
    {
        return bpm;
    }
private:
    double bpm = 120.0;
};
