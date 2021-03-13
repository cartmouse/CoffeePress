/*
  ==============================================================================

    CoffeePress.h
    Created: 13 Mar 2021 11:55:29am
    Author:  tom-c

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CoffeePress
{
public:
    CoffeePress();
    ~CoffeePress() {};
    float CompressSample(float data, float threshold, float ratio, float attackTime, float releaseTime, float knee);
private:
    juce::dsp::DelayLine<float> buffer;
    float timeAverage, rms, gain;
};