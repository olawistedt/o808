/*
  ==============================================================================

    "VA808BassDrum.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 15th July 2021
    Author:  Cameron Smith, UoE s1338237

    Class used for compartmentalizing the virtual analogue model of the TR-808
    bass drum. Added to allow for flexibility and ease of transportation for use
    in other environments.

  ==============================================================================
*/

#pragma once

#include "VA808Components/TriggerLogic.h"
#include "VA808Components/PulseShaper.h"
#include "VA808Components/RetriggeringPulse.h"
#include "VA808Components/BridgedTNetwork.h"
#include "VA808Components/FeedbackBuffer.h"
#include "VA808Components/ToneStage.h"
#include "VA808Components/LevelStage.h"
#include "VA808Components/OutputStage.h"

class VA808BassDrum
{
public:
    /**
     Gives all the bass drum components the sample rate and subsequently sets them up ready for use

     Must be called in the prepare to play function, before audio processing begins

     @param sampling frequency in Hz
     */
    void setSampleRate(float sampleRate);
    
    /**
     The playback trigger for the bass drum

     Should be called when a note is pressed and the velocity defines the level of accent

     @param the velocity of the note press
     */
    void activate(float velocity);

    /**
     Get next sample from the bass drum model

     Should be called in the process loop per sample
     */
    float process();

    /**
     Function for update the private variables that control the parameters of the bass drum

     @param level parameter between 0 and 1
     @param tone parameter between 0 and 1
     @param decay parameter between 0 and 1
     @param tuning parameter between 0 and 1
     */
    void updateParams(float _level, float _tone, float _decay, float _tuning, float _buttonState);

    void updateDecay();

private:
    /// Instance of the TriggerLogic class for the trigger logic process block
    TriggerLogic triggerLogic;

    /// Instance of the PulseShaper class for the pulse shaper process block
    PulseShaper pulseShaper;

    /// Instance of the RetriggeringPulse class for the retriggering pulse process block
    RetriggeringPulse retriggeringPulse;

    /// Instance of the PulseShaper class for the bridged-T network process block
    BridgedTNetwork bridgedTNetwork;

    /// Instance of the FeedbackBuffer class for the feedback buffer process block
    FeedbackBuffer feedbackBuffer;

    /// Instance of the ToneStage class for the tone stage process block
    ToneStage toneStage;

    /// Instance of the LevelStage class for the level stage process block
    LevelStage levelStage;

    /// Instance of the OutputStage class for the output stage process block
    OutputStage outputStage;


    /// Current value of level parameter, updated from the GUI
    float level;

    /// Current value of tone parameter, updated from the GUI
    float tone;

    /// Current value of decay parameter, updated from the GUI
    float decay;

    /// Current value of tuning parameter, updated from the GUI
    float tuning;

    bool decayLimiterActive = true;
};