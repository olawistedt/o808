/*
  ==============================================================================

    "PluginProcessor.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 24th June 2021
    Author:  Cameron Smith, UoE s1338237

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DrumSynthesiser.h"

//==============================================================================
/**
*/
class VA808BassDrumAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    VA808BassDrumAudioProcessor();
    ~VA808BassDrumAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState parameters;

private:
    /// Main instance of the synthesizer class
    juce::Synthesiser drumSynth;

    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    void setParams();

    /// Number of voices the synthesizer has
    int voiceCount = 1;

    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VA808BassDrumAudioProcessor)
};
