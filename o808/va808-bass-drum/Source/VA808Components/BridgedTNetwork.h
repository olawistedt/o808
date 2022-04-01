/*
  ==============================================================================

    "BridgedTNetwork.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 24th June 2021
    Author:  Cameron Smith, UoE s1338237

    Class that represents the bridged-T network of the VA model

  ==============================================================================
*/

#pragma once

//#include<JuceHeader.h>
#include"BridgedTComponents/PulseShaperInput.h"
#include"BridgedTComponents/FeedbackBufferInput.h"
#include"BridgedTComponents/RetriggeringPulseInput.h"

class BridgedTNetwork
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
    Function for updating value of effective resistance,

    @param voltage value at node, v_c, in the circuit diagram
    */
    void updateEffectiveResistance(float v_c);

    //--------------------------------------------------------------------------
    /**
    Function for updating coefficients of all the inputs to the network,

    First recalculates the parallel resistive value using current effective resistance
    Subsequently calls functions in all the input classes using the stored private variable r_eff
    */
    void updateCoefficients();

    //--------------------------------------------------------------------------
    /**
    Updates the past samples of the feedback buffer classes
    */
    void updateFeedbackBuffer(float v_fb);

    //--------------------------------------------------------------------------
    /**
    Get current sample of the bridged-T network process block
    */
    float process(float v_plus, float v_rp);

    //--------------------------------------------------------------------------
    /**
    Function for the op-amp clip, currently just a hard clip at the supplied voltage
    */
    void opAmpClip();


    //--------------------------------------------------------------------------
    /**
    After the main processing for calculating the current sample has taken place, update the coefficients of the components

    First update intermediate node coefficients, calculate v_c, update r_eff and then update the main coefficients
    */
    void postprocessUpdate(float v_plus, float v_fb, float v_rp);


    void updateTuning(float tuningParam);

private:
    /// constant used in bilinear transformation
    float K = 0.0f;

    /// value of capacitor C41 (15 nF)
    float c41 = static_cast<float>(1.5e-8);

    /// value of capacitor C42 (15 nF)
    float c42 = static_cast<float>(1.5e-8);

    /// value of resistor R161 (1 MOhms)
    float r161 = 1.0e6;
    
    /// value of resistor R165 (47 kOhms)
    float r165 = 4.7e4;

    /// value of resistor R166 (6.8 kOhms)
    float r166 = 6.8e3;
    
    /// value of resistor R167 (1 MOhms)
    float r167 = 1e6;

    /// value of resistor R170 (470 kOhms)
    float r170 = 4.7e5;

    /// effective resistance of the r165 + r166 branch with nonlinear contributions from leakage
    float r_eff;
    
    /// parallel combination, R161 || R_eff || R70
    float r_prl1;

    /// parallel combination, R161 || R_eff
    float r_prl2;

    /// parallel combination, R170 || R_eff
    float r_prl3;

    /// voltage being provided to the bridged-T op-amp
    float B2 = 15.0f;

    /// instantaneous output voltage of the bridged-T network, recalculated every process call
    float v_bt = 0.0f;

    HbtOne h_btOne;

    HbtTwo h_btTwo;

    HbtThree h_btThree;

    HcOne h_cOne;

    HcTwo h_cTwo;
    
    HcThree h_cThree;

    float alpha = 14.315f;

    float V0 = -0.5560f;

    float m = static_cast<float>(1.4765e-5);

    float interpLow = 5.3762e4;

    float interpHigh = 5.38e4;

    float interpC = 5.3775e4;

    float v_fbPrev1 = 0.0f;
};


//=======================================





