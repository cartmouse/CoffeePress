/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/**
*/
class CoffeePressAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CoffeePressAudioProcessorEditor (CoffeePressAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~CoffeePressAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CoffeePressAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& params;

    juce::Slider thresholdSlider, slopeSlider, kneeSlider, attackSlider, releaseSlider;
    juce::Label thresholdLabel, slopeLabel, kneeLabel, attackLabel, releaseLabel;
    std::unique_ptr<SliderAttachment> thresholdAttachment, slopeAttachment, kneeAttachment, attackAttachment, releaseAttachment;

    void addSlider(juce::String name, juce::String labelText, juce::Slider& slider, juce::Label& label, std::unique_ptr<SliderAttachment>& attachment);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoffeePressAudioProcessorEditor)
};
