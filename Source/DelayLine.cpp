/*
  ==============================================================================

    DelayLine.cpp
    Created: 1 Dec 2025 5:14:12pm
    Author:  Nico

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelayLine.h"

// Allocates memory
void DelayLine::setMaximumDelayInSamples(int maxLengthInSamples)
{
    jassert(maxLengthInSamples > 0);

    int paddedLength = maxLengthInSamples + 2;
    if (bufferLength < paddedLength) {
        bufferLength = paddedLength;
        buffer.reset(new float[size_t(bufferLength)]);
    }
}

// Clears the delay line and resets all state. This should be called before first usage.
void DelayLine::reset() noexcept
{
    writeIndex = bufferLength - 1;
    for (size_t i = 0; i < size_t(bufferLength); ++i) {
        buffer[i] = 0.0f;
    }
}

// Places a new sample into the delay line, overwriting the previous oldest element. 
void DelayLine::write(float input) noexcept
{
    jassert(bufferLength > 0);

    writeIndex ++;
    if (writeIndex >= bufferLength) {
        writeIndex = 0;
    }

    buffer[size_t(writeIndex)] = input;
}

// Uses hermite interpolation to read, allowing more accurate representation of fractional values
float DelayLine::read(float delayInSamples) const noexcept
{
    //jassert(delayInSamples >= 1.0f);
    //jassert(delayInSamples <= bufferLength - 2.0f);

    // Convert delay time to an integer value for calculation
    int integerDelay = int(delayInSamples);

    // Calculate indices
    int readIndexA = writeIndex - integerDelay + 1;
    int readIndexB = readIndexA - 1;
    int readIndexC = readIndexA - 2;
    int readIndexD = readIndexA - 3;

    // Check for negative index value
    if (readIndexD < 0) {
        readIndexD += bufferLength;
        if (readIndexC < 0) {
            readIndexC += bufferLength;
            if (readIndexB < 0) {
                readIndexB += bufferLength;
                if (readIndexA < 0) {
                    readIndexA += bufferLength;
                }
            }
        }
    }

    // Read the samples at the calculated indices from the buffer.
    float sampleA = buffer[size_t(readIndexA)];
    float sampleB = buffer[size_t(readIndexB)];
    float sampleC = buffer[size_t(readIndexC)];
    float sampleD = buffer[size_t(readIndexD)];

    // fraction represents the weight
    float fraction = delayInSamples - float(integerDelay);

    // Calculate the slope
    float slope0 = (sampleC - sampleA) * 0.5f;
    float slope1 = (sampleD - sampleB) * 0.5f;

    // Calculate coefficients
    float v = sampleB - sampleC;
    float w = slope0 + v;
    float a = w + v + slope1;
    float b = w + a;
    float stage1 = a * fraction - b;
    float stage2 = stage1 * fraction + slope0;
    return stage2 * fraction + sampleB;
}

float DelayLine::readAtIndex(int index) const noexcept
{
    jassert(index >= 0 && index < bufferLength);
    return buffer[index];
}