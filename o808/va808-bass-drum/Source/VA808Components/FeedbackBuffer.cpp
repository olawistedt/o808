/*
  ==============================================================================

    "FeedbackBuffer.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 25th June 2021
    Author:  Cameron Smith, UoE s1338237

    ----------------------see header file for description-----------------------
  ==============================================================================
*/

#include "FeedbackBuffer.h"
#include <math.h>

void FeedbackBuffer::setSampleRate(float sampleRate)
{
    // constant used in bilinear transformation substitution
    K = 2.0f * sampleRate;

    updateCoefficients(decay);
}

float FeedbackBuffer::process(float v_bt)
{
    // calculating the next step from the feedback buffer difference equation
    float v_fb = B0 * v_bt + B1 * v_btPrev1 - A1 * v_fbPrev1;

    // updating past sample variables
    v_btPrev1 = v_bt;
    v_fbPrev1 = v_fb;

    // feedback buffer output op-amp clip
    if (v_bt > B2 || v_bt < -B2)
    {
        //v_bt = B2;
        v_bt = 15.0f * tanh(v_bt / 15.0f);
    }

    return v_fb;
    
}

void FeedbackBuffer::updateCoefficients(float _decay)
{
    decay = _decay;
    
    // calculating coefficients of continuous-time transfer function of the feedback buffer stage
    float beta1 = -r169 * vr6 * decay * c43;
    float beta0 = -r169;
    float alpha1 = r164 * (r169 + vr6 * decay) * c43;
    float alpha0 = r164;

    // calculating discretized coefficients for feedback buffer
    A0 = K * alpha1 + alpha0;
    B0 = (K * beta1 + beta0) / A0;
    B1 = (beta0 - K * beta1) / A0;
    A1 = (alpha0 - K * alpha1) / A0;
}