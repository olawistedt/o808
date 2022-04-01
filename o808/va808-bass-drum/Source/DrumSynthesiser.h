/*
  ==============================================================================

    "DrumSynthesiser.h"
    Roland TR-808 Virtual Analogue Modelling - MSc Project
    
    Created: 24th June 2021
    Author:  Cameron Smith, UoE s1338237

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "VA808BassDrum.h"
#include "Overdrive.h"


// ===========================
// ===========================
// SOUND
class DrumSynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote      (int) override      { return true; }
    //--------------------------------------------------------------------------
    bool appliesToChannel   (int) override      { return true; }
};




// =================================
// =================================
// SYNTHESISER VOICE

/*!
 @class BassDrumSynthVoice
 @abstract struct defining the DSP associated with a specific voice.
 @discussion multiple BassDrumSynthVoice objects will be created by the Synthesiser so that it can be played polyphicially
 
 @namespace none
 @updated 2019-06-18
 */
class DrumSynthVoice : public juce::SynthesiserVoice
{
public:
    DrumSynthVoice() {}
    //--------------------------------------------------------------------------
    /**
     What should be done when a note starts

     @param midiNoteNumber
     @param velocity
     @param SynthesiserSound unused variable
     @param / unused variable
     */
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override;

    //--------------------------------------------------------------------------
    /// Called when a MIDI noteOff message is received
    /**
     What should be done when a note stops

     @param / unused variable
     @param allowTailOff bool to decie if the should be any volume decay
     */
    void stopNote(float /*velocity*/, bool allowTailOff) override;
    
    //--------------------------------------------------------------------------
    /// Prepare to play function
    /**
     What should be done when a note stops

     @param sampling frequency in Hz
     */
    void prepareToPlay(double sampleRate);

    //--------------------------------------------------------------------------
    /**
     The Main DSP Block: Put your DSP code in here
     
     If the sound that the voice is playing finishes during the course of this rendered block, it must call clearCurrentNote(), to tell the synthesiser that it has finished

     @param outputBuffer pointer to output
     @param startSample position of first sample in buffer
     @param numSamples number of smaples in output buffer
     */
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

    //--------------------------------------------------------------------------
    void pitchWheelMoved(int) override {}
    //--------------------------------------------------------------------------
    void controllerMoved(int, int) override {}
    //--------------------------------------------------------------------------
    /**
     Can this voice play a sound. I wouldn't worry about this for the time being

     @param sound a juce::SynthesiserSound* base class pointer
     @return sound cast as a pointer to an instance of YourSynthSound
     */
    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<DrumSynthSound*> (sound) != nullptr;
    }
    //--------------------------------------------------------------------------

    /**
     Used to update the current parameters of the TR-808 bass drum

     @param level parameter
     @param tone parameter
     @param decay parameter
     @param tuning parameter
     */
    void updateDrumParams(const float level, const float tone, const float decay, const float tuning, const float mix, const float gain, const float buttonState);

private:
    //--------------------------------------------------------------------------

    /// Instance of the TR-808 bass drum class
    VA808BassDrum bassDrum;

    /// Adjustable gain value for the output level
    float outputGain = 0.7f;

    /// Instance of the overdive class
    Overdrive overdrive;

    /// Smoothed value of the overdrive gain
    juce::SmoothedValue<float> smoothedGain;

    /// Smoothed value of the overdrive mix
    juce::SmoothedValue<float> smoothedMix;
};
