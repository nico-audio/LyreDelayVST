/*
  ==============================================================================

    DSP.h
    Created: 23 Oct 2025 9:39:41am
    Author:  Nico V.

  ==============================================================================
*/

#pragma once

#include <cmath>
inline void panningEqualPower(float panning, float& left, float& right)
{
    float theta = 0.7853981633974483f * (panning + 1.0f);
    left = std::cos(theta);
    right = std::sin(theta);
}