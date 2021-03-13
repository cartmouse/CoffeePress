/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CoffeePressAudioProcessorEditor::CoffeePressAudioProcessorEditor (CoffeePressAudioProcessor& p, juce::AudioProcessorValueTreeState& state)
    : AudioProcessorEditor (&p), audioProcessor (p), params(state)
{
    addSlider("threshold", "Threshold", thresholdSlider, thresholdLabel, thresholdAttachment);
    addSlider("ratio", "Ratio", slopeSlider, slopeLabel, slopeAttachment);
    addSlider("knee", "knee", kneeSlider, kneeLabel, kneeAttachment);
    addSlider("attack", "attack", attackSlider, attackLabel, attackAttachment);
    addSlider("release", "release", releaseSlider, releaseLabel, releaseAttachment);
    setSize (400, 300);
}

CoffeePressAudioProcessorEditor::~CoffeePressAudioProcessorEditor()
{
}

//==============================================================================
void CoffeePressAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void CoffeePressAudioProcessorEditor::resized()
{
    thresholdSlider.setBounds(100, 0, 200, 50);
    slopeSlider.setBounds(100, 50, 200, 50);
    kneeSlider.setBounds(100, 100, 200, 50);
    attackSlider.setBounds(100, 150, 200, 50);
    releaseSlider.setBounds(100, 200, 200, 50);
}

void CoffeePressAudioProcessorEditor::addSlider(juce::String name, juce::String labelText, juce::Slider& slider, juce::Label& label, std::unique_ptr<SliderAttachment>& attachment)
{
    addAndMakeVisible(slider);
    attachment.reset(new SliderAttachment(params, name, slider));
    label.setText(labelText, juce::dontSendNotification);
    label.attachToComponent(&slider, true);
    addAndMakeVisible(label);
}
