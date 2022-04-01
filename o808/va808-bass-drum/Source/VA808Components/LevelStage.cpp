/*
  ==============================================================================

    "LevelStage.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 25th June 2021
    Author:  Cameron Smith, UoE s1338237

    ----------------------see header file for description-----------------------
  ==============================================================================
*/

#include "LevelStage.h"

void LevelStage::setSampleRate(float sampleRate)
{
    // constant used in bilinear transformation substitution
    K = 2.0f * sampleRate;

    updateCoefficients(0.4f);
}

float LevelStage::process(float v_to)
{
    // calculating the next step from the tone stage difference equation
    float v_lev = (B0 * v_to) + (B1 * v_toPrev1) - (A1 * v_levPrev1);

    // updating past sample variables
    v_toPrev1 = v_to;
    v_levPrev1 = v_lev;

    return v_lev;

}

void LevelStage::updateCoefficients(float _level)
{
    level = _level;

    // calculating coefficients of continuous-time transfer function of the tone stage
    float beta1 = vr4 * level * c47;
    float alpha1 = vr4 * c47;

    // calculating discretized coefficients for tone stage
    A0 = K * alpha1 + 1;
    B0 = (K * beta1) / A0;
    B1 = (-K * beta1) / A0;
    A1 = (1.0f - K * alpha1) / A0;
}