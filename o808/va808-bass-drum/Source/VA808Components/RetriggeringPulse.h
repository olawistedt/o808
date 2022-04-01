/*
  ==============================================================================

    "RetriggeringPulse.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 30th June 2021
    Author:  Cameron Smith, UoE s1338237

    Class that represents the "retriggering pulse" block of the VA model, after 
    being given the sample rate the continuous-time coefficients will be 
    evaluated and subsequently the discrete-time coefficients through the 
    bilinear transform. The process function is called to get the next sample 
    through the difference equation.

    v_env    ---|low shelf filter|--->    v_rpc    ---|NL approx|--->     v_rp
       ^                                                                   ^
     input                                                               output
  ==============================================================================
*/

#pragma once

//#include <JuceHeader.h>

class RetriggeringPulse
{
public:

    //--------------------------------------------------------------------------
    /**
    Updates sample rate in the class private variables,

    Subsequently updates the discrete transfer function coefficients

    @param sample rate in Hz
    */
    void setSampleRate(float SR);

    //--------------------------------------------------------------------------
    /**
    Get current sample of the retriggering pulse process block

    @param the input voltage to the process block (output of the envelope generator)
    */
    float process(float v_env);

private:
    /// value of capacitor C39 (33 nF)
    float c39 = static_cast<float>(3.3e-8);

    /// value of resistor R161 (1 MOhms)
    float r161 = 1.0e6;

    /// coefficients of the discretized transfer function for the retriggering pulse core
    float A0 = 0.0f;
    float B0 = 0.0f;
    float B1 = 0.0f;
    float A1 = 0.0f;

    /// previous sample of input, v_env(n-1)
    float v_envPrev1 = 0.0f;

    /// previous sample of output of core, v_rpc(n-1) -- before nonlinear approximation
    float v_rpcPrev1 = 0.0f;

};