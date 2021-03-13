/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CoffeePressAudioProcessor::CoffeePressAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
    ,
    state(*this, nullptr, juce::Identifier("params"), {
            std::make_unique<juce::AudioParameterFloat>(
                "threshold",
                "Threshold",
                juce::NormalisableRange<float>(-60.0f, 20.0f, 0.01f),
                10.0f),
            std::make_unique<juce::AudioParameterFloat>(
                "ratio",
                "Ratio",
                juce::NormalisableRange<float>(1.0f, 20.0f, 0.01f),
                2.0f),
            std::make_unique<juce::AudioParameterFloat>(
                "knee",
                "Knee",
                juce::NormalisableRange<float>(0.0f, 24.0f, 0.01f),
                0.0f),
            std::make_unique<juce::AudioParameterFloat>(
                "attack",
                "Attack",
                juce::NormalisableRange<float>(0.01f, 500.0f, 0.01f),
                100.0f),
            std::make_unique<juce::AudioParameterFloat>(
                "release",
                "Reelase",
                juce::NormalisableRange<float>(0.01f, 2000.0f, 0.01f),
                500.0f),
        }
    )
#endif
{
}

CoffeePressAudioProcessor::~CoffeePressAudioProcessor()
{
}

//==============================================================================
const juce::String CoffeePressAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CoffeePressAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CoffeePressAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CoffeePressAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CoffeePressAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CoffeePressAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CoffeePressAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CoffeePressAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CoffeePressAudioProcessor::getProgramName (int index)
{
    return {};
}

void CoffeePressAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CoffeePressAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int channel = 0; channel < getTotalNumOutputChannels(); channel++)
    {
        allPresses.add(CoffeePress());
    }

    thresholdParam = state.getRawParameterValue("threshold");
    slopeParam = state.getRawParameterValue("ratio");
    kneeParam = state.getRawParameterValue("knee");
    attackParam = state.getRawParameterValue("attack");
    releaseParam = state.getRawParameterValue("release");
}

void CoffeePressAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CoffeePressAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CoffeePressAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    float at = 1 - std::pow(juce::MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (*attackParam / 1000.0f));
    float rt = 1 - std::pow(juce::MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (*attackParam / 1000.0f));

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        for (int channel = 0; channel < getTotalNumOutputChannels(); channel++)
        {
            auto* data = buffer.getWritePointer(channel);
            CoffeePress* press = &allPresses.getReference(channel);
            data[i] = press->CompressSample(data[i], *thresholdParam, *slopeParam, at, rt, *kneeParam);
        }
    }
}

//==============================================================================
bool CoffeePressAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CoffeePressAudioProcessor::createEditor()
{
    return new CoffeePressAudioProcessorEditor (*this, state);
}

//==============================================================================
void CoffeePressAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto stateTree = state.copyState();
    std::unique_ptr<juce::XmlElement> xml(stateTree.createXml());
    copyXmlToBinary(*xml, destData);
}

void CoffeePressAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr && xmlState->hasTagName(state.state.getType())) {
        state.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CoffeePressAudioProcessor();
}
