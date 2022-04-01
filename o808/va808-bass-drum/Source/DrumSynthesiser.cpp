/*
  ==============================================================================

    "DrumSynthesiser.cpp"
    Roland TR-808 Virtual Analogue Modelling - MSc Project

    Created: 24th June 2021
    Author:  Cameron Smith, UoE s1338237

    ----------------------see header file for description-----------------------
  ==============================================================================
*/

#include "DrumSynthesiser.h"


void DrumSynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    // send a message to the bass drum class to start a hit of the bass drum, independant of MIDI note
    bassDrum.activate(velocity);
}

void DrumSynthVoice::stopNote(float /*velocity*/, bool allowTailOff)
{

}

void DrumSynthVoice::prepareToPlay(double sampleRate)
{
    bassDrum.setSampleRate(sampleRate);
    overdrive.setSampleRate(sampleRate);

    // setting up smoothed values for the gain and mix of the overdrive
    smoothedGain.reset(sampleRate, 0.01);
    smoothedGain.setCurrentAndTargetValue(0.0);
    smoothedMix.reset(sampleRate, 0.01);
    smoothedMix.setCurrentAndTargetValue(0.0);
}

void DrumSynthVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    // iterate through the necessary number of samples (from startSample up to startSample + numSamples)
    for (int sampleIndex = startSample; sampleIndex < (startSample + numSamples); sampleIndex++)
    {
        // get next output sample from va808 bass drum model
        float v_out = bassDrum.process();


        //===========================================
        //APPLYING OVERDRIVE

        // updating the mix and gain variables from the smoothed values
        float overdriveMix = smoothedMix.getNextValue();
        float overdriveGain = smoothedGain.getNextValue();

        // store the dry sample before overdive, reduce volume
        float drySample = v_out * outputGain;

        // get the overdriven sample
        float overdrivenSample = overdrive.process(v_out * outputGain * (1.f + overdriveGain * 4.0f));

        // mix between the two to get the current output sample
        float currentSample = (1.0f - overdriveMix) * drySample + overdriveMix * overdrivenSample;

        //===========================================
        //WRITING OUTPUT TO BUFFER

        // for each channel, write the currentSample float to the output
        for (int chan = 0; chan < outputBuffer.getNumChannels(); chan++)
        {
            // The output sample is scaled by 0.2 so that it is not too loud by default
            outputBuffer.addSample(chan, sampleIndex, currentSample);
        }
        
    }
}

void DrumSynthVoice::updateDrumParams(const float level, const float tone, const float decay, const float tuning, const float mix, const float gain, const float buttonState)
{
    // hand the parameters to the bass drum class
    bassDrum.updateParams(level, tone, decay, tuning, buttonState);

    // overdrive parameters just need to be given as the targets for smoothed values
    smoothedMix.setTargetValue(mix);
    smoothedGain.setTargetValue(gain);

}