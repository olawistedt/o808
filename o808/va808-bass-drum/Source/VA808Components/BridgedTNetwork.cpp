/*
  ==============================================================================

    "BridgedTNetwork.cpp"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 24th June 2021
    Author:  Cameron Smith, UoE s1338237

    ----------------------see header file for description-----------------------
  ==============================================================================
*/

#include <math.h>
#include "BridgedTNetwork.h"

void BridgedTNetwork::setSampleRate(float SR)
{
    // set K constant with sample rate
    K = 2.0f * SR;

    // set an initial approximate value for r_eff as the series combination of the two resistors
    r_eff = r165 + r166;

    // updates the discrete time difference function coefficients
    updateCoefficients();
}

void BridgedTNetwork::updateEffectiveResistance(float v_c)
{
    float i_c = -log(1.0f + exp(-alpha * (v_c - V0)))* m / alpha;

    if (v_c < -0.1f)
    {
        r_eff = (v_c * r166 * (r165 + r166)) / (v_c * (r165 + r166) - r165 * (v_c - r166 * i_c));
    }
    else if (v_c < 0.2f)
    {
        r_eff = ((interpHigh - interpLow) * v_c / 0.3f) + interpC;
    }
    else
    {
        r_eff = r165 + r166;
    }

}

void BridgedTNetwork::updateCoefficients()
{
    // firstly updating the parallel resistance values from r_eff
    r_prl1 = 1.0f / ((1.0f / r161) + (1.0f / r_eff) + (1.0f / r170));
    r_prl2 = 1.0f / ((1.0f / r161) + (1.0f / r_eff));
    r_prl3 = 1.0f / ((1.0f / r170) + (1.0f / r_eff));

    // then calling the functions in the input classes to calculate coefficients
    h_btOne.calculateCoefficients(K, r_prl1, r167, c41, c42);
    h_btTwo.calculateCoefficients(K, r_prl2, r167, r170, c41, c42);
    h_btThree.calculateCoefficients(K, r_prl3, r161, r167, c41, c42);

}

void BridgedTNetwork::updateFeedbackBuffer(float v_fb)
{
    h_btTwo.updatePastSamples(v_fb);
}

float BridgedTNetwork::process(float v_plus, float v_rp)
{
    float v_btOne = h_btOne.process(v_plus);
    float v_btTwo = h_btTwo.process();
    float v_btThree = h_btThree.process(v_rp);
    
    // calculating output from superposition of the bridged-T inputs
    v_bt = v_btOne + v_btTwo + v_btThree;

    // bridged-T output op-amp clip
    opAmpClip();

    // return current output of bridged-T network
    return v_bt;

}

void BridgedTNetwork::opAmpClip()
{
    // hard limit on the value of the bridged-T output defined by the value being supplied to the op-amp
    if (v_bt > B2 || v_bt < -B2)
    {
        v_bt = 15.0f * tanh(v_bt / 15.0f);
    }

}

void BridgedTNetwork::postprocessUpdate(float v_plus, float v_fb, float v_rp)
{
    // calculating the coefficients for the h_c transfer function using the previous values of r_eff
    h_cOne.calculateCoefficients(K, r_prl1, r167, c41, c42);
    h_cTwo.calculateCoefficients(K, r_prl2, r167, r170, c41, c42);
    h_cThree.calculateCoefficients(K, r_prl3, r161, r167, c41, c42);

    float v_cOne = h_cOne.process(v_plus);
    float v_cTwo = h_cTwo.process(v_fbPrev1);
    float v_cThree = h_cThree.process(v_rp);

    v_fbPrev1 = v_fb;

    // calculating v_c as a superposition of the three input contributions
    float v_c = v_cOne + v_cTwo + v_cThree;

    // using the computed value of v_c to update the effective resistance (nonlinear approx)
    updateEffectiveResistance(v_c);

    // updating the main coefficients with new effective resistance
    updateCoefficients();
}

void BridgedTNetwork::updateTuning(float tuningParam)
{
    if (tuningParam < 0.5f)
    {
        r165 = 24000.0f + (tuningParam * 2.0f * 23000.0f);
    }
    else
    {
        r165 = 47000.0f + ((tuningParam - 0.5f) * 2.0f * 53000.0f);
    }
}

//=======================================




