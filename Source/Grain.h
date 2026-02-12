/*
  ==============================================================================

    Grain.h
    Created: 9 Jan 2026 2:43:57pm
    Author:  Nico V.

  ==============================================================================
*/

#pragma once

struct Grain
{
    float startIndex = 0.0f;
    float grainIndexPosition = 0.0f;          // Play head position
    float stepSize = 1.0f;                    // Playback rate (pitch)
    int grainDuration = 0;
    int samplesPlayed = 0;
    bool isActive = false;

    void reset()
    {
        isActive = false;
        samplesPlayed = 0;
        grainIndexPosition = 0.0f;
    }
};
