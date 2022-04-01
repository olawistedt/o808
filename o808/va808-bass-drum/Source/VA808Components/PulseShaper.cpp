/*
  ==============================================================================

    "PulseShaper.cpp"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 24th June 2021
    Author:  Cameron Smith, UoE s1338237

    ----------------------see header file for description-----------------------
  ==============================================================================
*/

#include "PulseShaper.h"
#include <math.h>


void PulseShaper::setSampleRate(float sampleRate)
{
    // constant used in bilinear transformation substitution
    float K = 2.0f * sampleRate;

    // calculating coefficients of continuous-time transfer function of the shelf filter core
    float beta1 = r162 * r163 * c40;
    float beta0 = r162;
    float alpha1 = r162 * r163 * c40;
    float alpha0 = r162 + r163;

    // calculating discretized coefficients for pulse shaper filter core
    A0 = K * alpha1 + alpha0;
    B0 = (K * beta1 + beta0) / A0;
    B1 = (beta0 - K * beta1) / A0;
    A1 = (alpha0 - K * alpha1) / A0;

}

float PulseShaper::process(float v_trig)
{
    // calculating the next step from the pulse shaper difference equation
    float v_ps = (B0 * v_trig) + (B1 * v_trigPrev1) - (A1 * v_psPrev1);

    // updating past sample variables
    v_psPrev1 = v_ps;
    v_trigPrev1 = v_trig;

    // implementation of the memoryless nonlinear approximation of the diode's function
    float v_plus = v_ps;
    if (v_plus < 0.0f)
    {
        v_plus = 0.71f * (float(exp(v_plus)) - 1.0f);
    }

    // return v_plus, value at bridged-T op-amp's inverting input
    return v_plus;

}