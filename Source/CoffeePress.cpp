/*
  ==============================================================================

    CoffeePress.cpp
    Created: 13 Mar 2021 11:55:29am
    Author:  tom-c

  ==============================================================================
*/

#include "CoffeePress.h"

CoffeePress::CoffeePress()
{
    buffer = CircularBuffer(150, 20);
    timeAverage = 0.01;
    rms = 0;
    gain = 1;
}

float CoffeePress::CompressSample(float data, float threshold, float ratio, float attackTime, float releaseTime, float knee)
{
    // Detect gain - Calculate rms and convert to dB
    rms = (1 - timeAverage) * rms + timeAverage * std::pow(data, 2.0f);
    float dbRMS = 10 * std::log10(rms);

    // Reduce gain - convert ratio to slope. Find the required gain in dB (<0.0f). Convert to linear gain.
    float slope = 1 - (1 / ratio);
    float dbGain = std::min(0.0f, (slope * (threshold - dbRMS)));
    float newGain = std::pow(10, dbGain / 20);

    // Smooth transistion
    float coeff = newGain < gain ? attackTime : releaseTime;
    gain = (1 - coeff) * gain + coeff * newGain;

    // Get and set values to buffer
    float compressedSample = gain * buffer.getData();
    buffer.setData(data);
    buffer.nextSample();
    return compressedSample;
}