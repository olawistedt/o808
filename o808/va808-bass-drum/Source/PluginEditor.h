/*
  ==============================================================================

    "PluginEditor.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 24th June 2021
    Author:  Cameron Smith, UoE s1338237

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUIComponents/Background.h"
#include "GUIComponents/CustomDial.h"
#include "GUIComponents/CustomButton.h"

//==============================================================================
/**
*/
class VA808BassDrumAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    VA808BassDrumAudioProcessorEditor (VA808BassDrumAudioProcessor&);
    ~VA808BassDrumAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VA808BassDrumAudioProcessor& audioProcessor;

    // creating instance of background class which contains all static visual elements
    Background background;

    // a dial for controlling the bass drum level parameter
    CustomDial level;

    // a dial for controlling the bass drum tone parameter
    CustomDial tone;

    // a dial for controlling the bass drum decay parameter
    CustomDial decay;

    // a button for turning the decay limiting ability on and off
    CustomButton decayLimiter;

    // a dial for controlling the bass drum tuning (circuit bending modification)
    CustomDial tuning;

    // a dial for controlling the dry/wet mix of the overdrive
    CustomDial mix;

    // a dial for controlling the gain of the overdrive
    CustomDial gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VA808BassDrumAudioProcessorEditor)

};
