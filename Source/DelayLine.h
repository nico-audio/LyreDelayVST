/*
  ==============================================================================

    DelayLine.h
    Created: 1 Dec 2025 5:14:12pm
    Author:  Nico

  ==============================================================================
*/

#pragma once

#include <memory>

class DelayLine
{
public:
    void setMaximumDelayInSamples(int maxLengthInSamples);
    void reset() noexcept;

    void write(float input) noexcept;
    float read(float delayInSamples) const noexcept;

    int getBufferLength() const noexcept
    {
        return bufferLength;
    }

private:
    std::unique_ptr<float[]> buffer;
    int bufferLength = 0;
    int writeIndex = 0;   // where the most recent value was written
};