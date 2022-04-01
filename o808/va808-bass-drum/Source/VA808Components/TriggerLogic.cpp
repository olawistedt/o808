/*
  ==============================================================================

    "TriggerLogic.cpp"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 24th June 2021
    Author:  Cameron Smith, UoE s1338237

    ----------------------see header file for description-----------------------
  ==============================================================================
*/

#include "TriggerLogic.h"

#define M_PI 3.141592653589793238462643383279f


void TriggerLogic::updateSampleRate(float sampleRate)
{
    // updating pulse length in samples by mulptiplying 1ms time duration and sample rate
    pulseLengthInSamples = round(pulseLengthInSeconds * sampleRate);

    // update envelope length in samples by multiplying 5ms time duration and sample rate
    int envLengthInSamples = round(envLengthInSeconds * sampleRate);

    // calculating a tail length for the envelope decay in samples
    int tailLengthInSamples = round(envTailLengthInSeconds * sampleRate);

    // calculating the total length of the envelope, main pulse and tail
    totalEnvLength = envLengthInSamples + tailLengthInSamples;

    // creating a temporary vector for storing the pulse 
    std::vector<float> envelope;
    envelope.resize(totalEnvLength);

    // clearing and resizing envelope vector with total sample length
    v_env.clear();
    v_env.resize(totalEnvLength);

    // initializing previous value variable, v_env(n-1)
    float v_envPrev = 0;

    //=================================================
    // FILTERING THE PULSE (RISE)

    // setting first cutoff frequency value for filtering
    float cutoffFreq = 550.0f * M_PI;

    // precomputing difference equation coefficient
    float alpha = (cutoffFreq / sampleRate) / (1.0f + cutoffFreq / sampleRate);

    // performing first low pass filtering on the body of the pulse to achieve a rise
    for (int n = 0; n < envLengthInSamples; n++)
    {
        envelope[n] = envAmp;
        envelope[n] = alpha * envelope[n] + (1.0f - alpha) * v_envPrev;
        v_envPrev = envelope[n];
        v_env[n] = envelope[n];
    }

    //=================================================
    // FILTERING THE PULSE (FALL/DECAY)

    // setting new cutoff frequency value for filter
    cutoffFreq = 1400.0f * M_PI;
    
    // updating difference coefficient
    alpha = (cutoffFreq / sampleRate) / (1.0f + cutoffFreq / sampleRate);

    // performing second low pass filtering on the tail of the pulse to shape the decay
    for (int n = envLengthInSamples; n < totalEnvLength; n++)
    {
        envelope[n] = 0;
        envelope[n] = alpha * envelope[n] + (1.0f - alpha) * v_envPrev;
        v_envPrev = envelope[n];

        //int index = 2 * envLengthInSamples - n + tailLength;
        int index = totalEnvLength - 1 - n + envLengthInSamples;

        // assigned to location in v_env with mirroring trick
        v_env[index] = 0.05f + envAmp - envelope[n];

    }
}

void TriggerLogic::setTriggerActive(float velocity)
{
    // check if trigger is not already active, if it is then nothing happens. too quick!
    // note: checks the envelope active status, as this is the longer of the two
    if (envActive == false)
    {
        // if trigger is not active, make it so
        pulseActive = true;
        envActive = true;

        // update the common trigger signal amplitude (4-5V), dependent on velocity
        v_ct = 4.0f + (velocity * 1.0f);

    }
}

float TriggerLogic::triggerProcess()
{
    if (pulseActive == true && pulseIndex < pulseLengthInSamples)
    {
        // currently active, increment index and return trigger signal amplitude
        pulseIndex++;
        return v_ct;
    }
    else if (pulseActive == true && pulseIndex >= pulseLengthInSamples)
    {
        // trigger pulse is finished, reset index and turn active state to false
        pulseIndex = 0;
        pulseActive = false;

        // return zero output
        return 0.0f;
    }
    else
    {
        // trigger pulse is not active therefore return zero output
        return 0.0f;
    }
}

float TriggerLogic::envProcess()
{
    if (envActive && envIndex < totalEnvLength)
    {
        // currently active, increment index
        envIndex++;

        // return envelope amplitude for current index from stored vector/wavetable
        int currentIndex = envIndex - 1;
        return v_env[currentIndex];
    }
    else if (envActive && envIndex >= totalEnvLength)
    {
        // reset index and turn active state to false
        envIndex = 0;
        envActive = false;

        // envelope has reached the end so return zero output
        return 0.0f;
    }
    else
    {
        // envelope is not active therefore return zero output
        return 0.0f;
    }
}

