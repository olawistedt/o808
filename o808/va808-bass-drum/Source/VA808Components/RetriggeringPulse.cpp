/*
  ==============================================================================

    "RetriggeringPulse.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 30th June 2021
    Author:  Cameron Smith, UoE s1338237

    ----------------------see header file for description-----------------------
  ==============================================================================
*/

#include <math.h>
#include "RetriggeringPulse.h"

void RetriggeringPulse::setSampleRate(float sampleRate)
{
    // constant used in bilinear transformation substitution
    float K = 2.0f * sampleRate;

    // calculating coefficients of continuous-time transfer function of the retriggering pulse core
    float beta1 = r161 * c39;
    float alpha1 = r161 * c39;

    // calculating discretized coefficients for retriggering pulse filter core
    A0 = K * alpha1 + 1.0f;
    B0 = (K * beta1) / A0;
    B1 = (-K * beta1) / A0;
    A1 = (1.0f - K * alpha1) / A0;

}

float RetriggeringPulse::process(float v_env)
{
    // calculating the next step from the "retriggering pulse core" difference equation
    float v_rpc = (B0 * v_env) + (B1 * v_envPrev1) - (A1 * v_rpcPrev1);

    // updating past sample variables
    v_rpcPrev1 = v_rpc;
    v_envPrev1 = v_env;

    // implementation of the memoryless nonlinear approximation of the diode's function
    float v_rp = v_rpc;
    if (v_rp < 0.0f)
    {
        v_rp = 0.71f * (float(exp(v_rp)) - 1.0f);
    }

    // return v_rp, value at retriggering pulse block's output
    return v_rp;

}
