/*
  ==============================================================================

    CircularBuffer.h
    Created: 13 Mar 2021 1:15:07pm
    Author:  tom-c

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CircularBuffer {
public:
    CircularBuffer();
    CircularBuffer(int bufferSize, int delayLength);
    float getData();
    void setData(float data);
    void nextSample();
private:
    juce::AudioSampleBuffer buffer;
    int writeIndex;
    int readIndex;
    int delayLength;
};