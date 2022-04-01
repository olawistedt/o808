/*
  ==============================================================================
    "PulseShaperInput.h"

    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 13th July 2021
    Author:  Cameron Smith, UoE s1338237

  ==============================================================================
*/

#pragma once
 
//==============================================================================
// MAIN OUTPUT CLASS

class HbtOne
{
public:
    void calculateCoefficients(float K, float r_prl1, float r167, float c41, float c42)
    {
        float beta2 = r_prl1 * r167 * c41 * c42;
        float beta1 = r_prl1 * c41 + r167*c41 + r_prl1 * c42;
        float beta0 = 1.0f;
        float alpha2 = r_prl1 * r167 * c41 * c42;
        float alpha1 = r_prl1 * (c41 + c42);
        float alpha0 = 1.0f;

        A0 = K * K * alpha2 + K * alpha1 + alpha0;

        B0 = (K * K * beta2 + K * beta1 + beta0) / A0;
        B1 = (2.0f * beta0 - 2.0f * K * K * beta2) / A0;
        B2 = (K * K * beta2 - K * beta1 + beta0) / A0;
        A1 = (2.0f * alpha0 - 2.0f * K * K * alpha2) / A0;
        A2 = (K * K * alpha2 - K * alpha1 + alpha0) / A0;
    }

    float process(float v_plus)
    {
        // calculate next step from the difference equation
        float v_bt1 = B0 * v_plus + B1 * v_plusPrev1 + B2 * v_plusPrev2 - A1 * v_bt1Prev1 - A2 * v_bt1Prev2;

        // update past sample variables for v_plus(n-1) and v_plus(n-2)
        v_plusPrev2 = v_plusPrev1;
        v_plusPrev1 = v_plus;

        // update past sample variables for v_bt1(n-1) and v_bt1(n-2)
        v_bt1Prev2 = v_bt1Prev1;
        v_bt1Prev1 = v_bt1;

        // return calculated value of current sample v_bt1(n)
        return v_bt1;
    }

private:
    /// coefficients of the discretized transfer functions for bridged-T network, H_bt1(z), input 1 from pulse shaper
    float A0 = 0.0f;
    float B0 = 0.0f;
    float B1 = 0.0f;
    float B2 = 0.0f;
    float A1 = 0.0f;
    float A2 = 0.0f;

    /// previous sample of input, v_plus(n-1)
    float v_plusPrev1 = 0.0f;

    /// second previous sample of input, v_plus(n-2)
    float v_plusPrev2 = 0.0f;

    /// previous sample of output, v_bt1(n-1)
    float v_bt1Prev1 = 0.0f;

    /// second previous sample of output, v_bt1(n-2)
    float v_bt1Prev2 = 0.0f;
};


//==============================================================================
// INTERMEDIATE NODE OUTPUT CLASS

class HcOne
{
public:
    void calculateCoefficients(float K, float r_prl1, float r167, float c41, float c42)
    {
        float beta2 = r_prl1 * r167 * c41 * c42;
        float beta1 = r_prl1 * (c41 + c42);
        float alpha2 = r_prl1 * r167 * c41 * c42;
        float alpha1 = r_prl1 * (c41 + c42);

        A0 = K * K * alpha2 + K * alpha1;

        B0 = (K * K * beta2 + K * beta1) / A0;
        B1 = (-2.0f * K * K * beta2) / A0;
        B2 = (K * K * beta2 - K * beta1) / A0;
        A1 = (2.0f - 2.0f * K * K * alpha2) / A0;
        A2 = (K * K * alpha2 - K * alpha1 + 1.0f) / A0;
    }

    float process(float v_plus)
    {
        // calculate next step from the difference equation
        float v_c1 = B0 * v_plus + B1 * v_plusPrev1 + B2 * v_plusPrev2 - A1 * v_c1Prev1 - A2 * v_c1Prev2;

        // update past sample variables for v_plus(n-1) and v_plus(n-2)
        v_plusPrev2 = v_plusPrev1;
        v_plusPrev1 = v_plus;

        // update past sample variables for v_c1(n-1) and v_c1(n-2)
        v_c1Prev2 = v_c1Prev1;
        v_c1Prev1 = v_c1;

        // return calculated value of current sample v_c1(n)
        return v_c1;
    }

private:
    /// coefficients of the discretized transfer functions for bridged-T network, H_c1(z), input 1 from pulse shaper
    float A0 = 0.0f;
    float B0 = 0.0f;
    float B1 = 0.0f;
    float B2 = 0.0f;
    float A1 = 0.0f;
    float A2 = 0.0f;

    /// previous sample of input, v_plus(n-1)
    float v_plusPrev1 = 0.0f;

    /// second previous sample of input, v_plus(n-2)
    float v_plusPrev2 = 0.0f;

    /// previous sample of output, v_c1(n-1)
    float v_c1Prev1 = 0.0f;

    /// second previous sample of output, v_c1(n-2)
    float v_c1Prev2 = 0.0f;
};