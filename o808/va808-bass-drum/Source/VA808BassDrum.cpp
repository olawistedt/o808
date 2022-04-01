/*
  ==============================================================================

    "VA808BassDrum.cpp"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 15th July 2021
    Author:  Cameron Smith, UoE s1338237

    Class used for compartmentalizing the virtual analogue model of the TR-808
    bass drum. Added to allow for flexibility and ease of transportation for use
    in other environments.

  ==============================================================================
*/

#include "VA808BassDrum.h"

void VA808BassDrum::setSampleRate(float sampleRate)
{
    triggerLogic.updateSampleRate(sampleRate);
    pulseShaper.setSampleRate(sampleRate);
    retriggeringPulse.setSampleRate(sampleRate);
    bridgedTNetwork.setSampleRate(sampleRate);
    feedbackBuffer.setSampleRate(sampleRate);
    toneStage.setSampleRate(sampleRate);
    levelStage.setSampleRate(sampleRate);
    outputStage.setSampleRate(sampleRate);
}

void VA808BassDrum::activate(float velocity)
{
    triggerLogic.setTriggerActive(velocity);
}

float VA808BassDrum::process()
{
    //===========================================
    //BRIDGED-T INPUT 1
    float v_trig = triggerLogic.triggerProcess();

    float v_plus = pulseShaper.process(v_trig);


    //===========================================
    //BRIDGED-T INPUT 3
    float v_env = triggerLogic.envProcess();

    float v_rp = retriggeringPulse.process(v_env);


    //===========================================
    //BRIDGED-T UPDATE
    float v_bt = bridgedTNetwork.process(v_plus, v_rp);


    //===========================================
    //BRIDGED-T INPUT 2
    float v_fb = feedbackBuffer.process(v_bt);

    bridgedTNetwork.updateFeedbackBuffer(v_fb);


    //===========================================
    //OUTPUT STAGES
    float v_to = toneStage.process(v_bt);

    float v_lev = levelStage.process(v_to);

    float v_out = outputStage.process(v_lev);


    //===========================================
    //FREQUENCY EFFECTS

    bridgedTNetwork.postprocessUpdate(v_plus, v_fb, v_rp);

    return v_out;
}

void VA808BassDrum::updateParams(float _level, float _tone, float _decay, float _tuning, float _buttonState)
{
    if (level != _level)
    {
        level = _level;
        levelStage.updateCoefficients(level);   
    }

    if (tone != _tone)
    {
        tone = _tone;
        toneStage.updateCoefficients(tone);
    }
    
    if (decay != _decay)
    {
        decay = _decay;
        updateDecay();
    }

    if (tuning != _tuning)
    {
        tuning = _tuning;
        bridgedTNetwork.updateTuning(tuning);
        updateDecay();
    }

    if (decayLimiterActive != bool(_buttonState))
    {
        decayLimiterActive = bool(_buttonState);

        updateDecay();
    }
    
}

void VA808BassDrum::updateDecay()
{
    if (decayLimiterActive)
    {
        if (tuning < 0.5f)
        {
            float ratio = tuning * 2.0f;

            // logarithmic mapping to mirror to logarithmic potentiometer in the TR-808 tone knob
            float y_m = 0.3f;
            float b = powf(1.0f / y_m - 1.0f, 2.0f);
            float a = 1.0f / (b - 1.0f);

            float logRatio = a * powf(b, ratio) - a;


            feedbackBuffer.updateCoefficients(decay * (0.16f + 0.84f * logRatio));
        }
        else if (tuning >= 0.5f)
        {
            float ratio = 1.0f - ((tuning - 0.5f) * 2.0f);

            // logarithmic mapping to mirror to logarithmic potentiometer in the TR-808 tone knob
            float y_m = 0.3f;
            float b = powf(1.0f / y_m - 1.0f, 2.0f);
            float a = 1.0f / (b - 1.0f);

            float logRatio = a * powf(b, ratio) - a;

            feedbackBuffer.updateCoefficients(decay * (0.22f + 0.78f * logRatio));
        }
    }
    else
    {
        feedbackBuffer.updateCoefficients(decay);
    }
}
