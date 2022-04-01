/*
  ==============================================================================

    "TriggerLogic.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 24th June 2021
    Author:  Cameron Smith, UoE s1338237

    Class that represents the trigger logic block of the VA model. When a note
    is played in the synthesiser class a 1ms square-shaped pulse is generated.
    Also manages the envelope generator in the block diagram by precomputing its
    shape and storing in a vector for playback.

  ==============================================================================
*/

#pragma once

// #include<JuceHeader.h>
#include<vector>

class TriggerLogic
{
public:

    //--------------------------------------------------------------------------
    /**
    Updates sample rate and consequently the pulse length in samples
    @param sample rate in Hz
    */
    void updateSampleRate(float sampleRate);

    //--------------------------------------------------------------------------
    /**
    Set the trigger logic to an active state. it will begin outputing a pulse
    @param velocity of the MIDI note played, determines amplitude of pulse
    */
    void setTriggerActive(float velocity);

    //--------------------------------------------------------------------------
    /**
    Get current sample of the trigger logic process block
    */
    float triggerProcess();

    //--------------------------------------------------------------------------
    /**
    Get current sample of the envelope generator process block
    */
    float envProcess();

private:

    //=================================================
    // PULSE VARIABLES

    /// length of the pulse in seconds (fixed as 1ms)
    float pulseLengthInSeconds = 0.001f;

    /// length of the pulse in samples (dependant on sample rate)
    float pulseLengthInSamples;

    /// common trigger signal voltage amplitude (4-14V)
    float v_ct = 0.0f;

    /// is the triggering pulse currently active
    bool pulseActive = false;

    /// index, used to keep track of progress through the pulse duration
    int pulseIndex = 0;

    //=================================================
    // ENVELOPE VARIABLES

    /// length of the envelope in seconds (fixed as 5ms)
    float envLengthInSeconds = 0.005f;

    /// length of the envelope decay in seconds (fixed as 1.25ms)
    float envTailLengthInSeconds = 0.00125f;

    /// total length of the envelope in samples, pulse + decay (dependant on sample rate)
    int totalEnvLength;

    /// amplitude of the envelope, fixed value
    float envAmp = 12.406f;

    /// is the envelope generator currently active
    bool envActive = false;

    /// index, used to keep track of progress through the envelope duration
    int envIndex = 0;

    /// pointer to v_env where the envelope data will be stored
    std::vector<float> v_env;

};
