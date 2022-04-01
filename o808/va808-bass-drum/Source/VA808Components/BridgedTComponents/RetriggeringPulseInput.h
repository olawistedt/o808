/*
  ==============================================================================
    "RetriggeringPulseInput.h"

    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 13th July 2021
    Author:  Cameron Smith, UoE s1338237

  ==============================================================================
*/

#pragma once

//==============================================================================
// MAIN OUTPUT CLASS

class HbtThree
{
public:
    void calculateCoefficients(float K, float r_prl3, float r161, float r167, float c41, float c42)
    {
        float beta1 = -r_prl3 * r167 * c41;
        float alpha2 = r_prl3 * r167 * r161 * c41 * c42;
        float alpha1 = r_prl3 * r161 * (c41 + c42);
        float alpha0 = r_prl3 + r161;

        A0 = K * K * alpha2 + K * alpha1 + alpha0;

        B0 = (K * beta1) / A0;
        B2 = (-K * beta1) / A0;
        A1 = (2.0f * alpha0 - 2.0f * K * K * alpha2) / A0;
        A2 = (K * K * alpha2 - K * alpha1 + alpha0) / A0;
    }

    float process(float v_rp)
    {
        // calculate next step from the difference equation
        float v_bt3 = B0 * v_rp + B2 * v_rpPrev2 - A1 * v_bt3Prev1 - A2 * v_bt3Prev2;

        // update past sample variables for v_rp(n-1) and v_rp(n-2)
        v_rpPrev2 = v_rpPrev1;
        v_rpPrev1 = v_rp;

        // update past sample variables for v_bt3(n-1) and v_bt3(n-2)
        v_bt3Prev2 = v_bt3Prev1;
        v_bt3Prev1 = v_bt3;

        // return calculated value of current sample v_bt3(n)
        return v_bt3;
    }

private:
    /// coefficients of the discretized transfer functions for bridged-T network, H_bt3(z), input 3 from retriggering pulse
    float A0 = 0.0f;
    float B0 = 0.0f;
    float B2 = 0.0f;
    float A1 = 0.0f;
    float A2 = 0.0f;

    /// previous sample of input, v_rp(n-1)
    float v_rpPrev1 = 0.0f;

    /// second previous sample of input, v_rp(n-2)
    float v_rpPrev2 = 0.0f;

    /// previous sample of output, v_bt3(n-1)
    float v_bt3Prev1 = 0.0f;

    /// second previous sample of output, v_bt3(n-2)
    float v_bt3Prev2 = 0.0f;
};


//==============================================================================
// INTERMEDIATE NODE OUTPUT CLASS

class HcThree
{
public:
    void calculateCoefficients(float K, float r_prl3, float r161, float r167, float c41, float c42)
    {
        float beta0 = r_prl3;
        float alpha2 = r_prl3 * r167 * r161 * c41 * c42;
        float alpha1 = r_prl3 * r161 * (c41 + c42);
        float alpha0 = r_prl3 + r161;

        A0 = K * K * alpha2 + K * alpha1 + alpha0;

        B0 = beta0 / A0;
        B1 = (2.0f * beta0) / A0;
        B2 = beta0 / A0;
        A1 = (2.0f * alpha0 - 2.0f * K * K * alpha2) / A0;
        A2 = (K * K * alpha2 - K * alpha1 + alpha0) / A0;
    }

    float process(float v_rp)
    {
        // calculate next step from the difference equation
        float v_c3 = B0 * v_rp + B1 * v_rpPrev1 + B2 * v_rpPrev2 - A1 * v_c3Prev1 - A2 * v_c3Prev2;

        // update past sample variables for v_rp(n-1) and v_rp(n-2)
        v_rpPrev2 = v_rpPrev1;
        v_rpPrev1 = v_rp;

        // update past sample variables for v_c3(n-1) and v_c3(n-2)
        v_c3Prev2 = v_c3Prev1;
        v_c3Prev1 = v_c3;

        // return calculated value of current sample v_c3(n)
        return v_c3;
    }

private:
    /// coefficients of the discretized transfer function for bridged-T network, H_c3(z), input 3 from retriggering pulse
    float A0 = 0.0f;
    float B0 = 0.0f;
    float B1 = 0.0f;
    float B2 = 0.0f;
    float A1 = 0.0f;
    float A2 = 0.0f;

    /// previous sample of input, v_rp(n-1)
    float v_rpPrev1 = 0.0f;

    /// second previous sample of input, v_rp(n-2)
    float v_rpPrev2 = 0.0f;

    /// previous sample of output, v_c3(n-1)
    float v_c3Prev1 = 0.0f;

    /// second previous sample of output, v_c3(n-2)
    float v_c3Prev2 = 0.0f;
};