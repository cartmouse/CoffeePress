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
    timeAverage = 0.01;
    rms = 0;
    gain = 1;
}

float CoffeePress::CompressSample(float data, float threshold, float ratio, float attackTime, float releaseTime, float knee)
{

}