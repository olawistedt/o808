/*
  ==============================================================================

    "ToneStage.cpp"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 25th June 2021
    Author:  Cameron Smith, UoE s1338237

    Class that represents the tone block of the VA model

  ==============================================================================
*/

#include "ToneStage.h"
#include <math.h>

void ToneStage::setSampleRate(float sampleRate)
{
    // constant used in bilinear transformation substitution
    K = 2.0f * sampleRate;

    updateCoefficients(0.8f);
}

float ToneStage::process(float v_bt)
{
    // calculating the next step from the tone stage difference equation
    float v_to = (B0 * v_bt) + (B1 * v_btPrev1) - (A1 * v_toPrev1);

    // updating past sample variables
    v_btPrev1 = v_bt;
    v_toPrev1 = v_to;

    return v_to;

}

void ToneStage::updateCoefficients(float _tone)
{

    // logarithmic mapping to mirror to logarithmic potentiometer in the TR-808 tone knob
    float y_m = 0.6f;
    float b = powf(1.0f / y_m - 1.0f, 2.0f);
    float a = 1.0f / (b - 1.0f);

    tone = a * powf(b, _tone) - a;

    // calculating the equivalent resistance of the network, R171 + (R172 || VR5) 
    float r_eq = r171 + r172 * vr5 * (1.0f - tone) / (r172 + vr5 * (1.0f - tone));

    // calculating coefficients of continuous-time transfer function of the tone stage
    float alpha1 = r_eq * c45;

    // calculating discretized coefficients for tone stage
    A0 = K * alpha1 + 1;
    B0 = 1.0f / A0;
    B1 = 1.0f / A0;
    A1 = (1.0f - K * alpha1) / A0;
}
