/*
  ==============================================================================

    "LevelStage.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 25th June 2021
    Author:  Cameron Smith, UoE s1338237

    Class that represents the level block of the VA model

  ==============================================================================
*/

#pragma once

class LevelStage
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
    Get current sample of the level process block

    @param the input voltage to the process block (output of the tone block)
    */
    float process(float v_to);


    void updateCoefficients(float _level);

private:
    float K;
    
    /// value of capacitor C47 (0.47 microF)
    float c47 = static_cast<float>(4.7e-7);

    /// maximum value of variable resistor VR4(100 kOhms)
    float vr4 = 1.0e5;

    /// VR4 mod [0,1] -- LEVEL KNOB, 0=full volume | 1=muted
    float level = 0.4f;

    /// coefficients of the discretized transfer function for level stage
    float A0 = 0.0f;
    float B0 = 0.0f;
    float B1 = 0.0f;
    float A1 = 0.0f;

    /// previous sample of input, v_to(n-1)
    float v_toPrev1 = 0.0f;

    /// previous sample of output, v_lev(n-1)
    float v_levPrev1 = 0.0f;

};
