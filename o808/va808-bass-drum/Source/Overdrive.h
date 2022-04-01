/*
  ==============================================================================

    "Overdrive.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 21th July 2021
    Author:  Cameron Smith, UoE s1338237

    This class manages the overdrive effect on the output of the bass drum.
    Based on the asymmetric diode clipper circuit

  ==============================================================================
*/

#pragma once

class Overdrive
{
public:
    //--------------------------------------------------------------------------
    /**
    Updates sample rate in the class private variables,

    Subsequently updates the necessary coefficients of the model 

    @param sample rate in Hz
    */
    void setSampleRate(float SR);


    //--------------------------------------------------------------------------
    /**
    Get current sample of the output of the assymetric diode clipper

    @param the input voltage to the overdrive circuit
    */
    float process(float input);


private:
    /// Resistance value (1000 Ohms)
    float resistance = 1000.0f;

    /// Capacitance value (33 nF)
    float capacitance = static_cast<float>(3.3e-8);
    
    /// Diode saturation current (amps)
    float satCurrent = static_cast<float>(2.52e-9);

    /// Diode thermal voltage (volts)
    float diodeThermalVoltage = 0.02583f;

    /// Diode ideality factor
    float diodeIdealityFac = 1.752f;

    /// Tolerance level for Newton-Raphson
    float tolerance = static_cast<float>(1.0e-9);

    /// The maximum number of iterations before breaking out of NR
    int maxIterations = 100;

    // STATE SPACE PARAMETERS
    float A;
    float B;
    float C;
    float D = 1.0f;
    float E = 0.0f;
    float F = 0.0f;
    float L = 1.0f;
    float M = 0.0f;
    float N = 0.0f;

    // DERIVED STATE SPACE PARAMETERS
    float Ha;
    float Hb;
    float K;

    /// Scalar variable, linear approximation of nonlinearity
    float G;

    // previous variable, x(n-1), i(n-1) and u(n-1)
    float xPrev = 0.0f;
    float iPrev = 0.0f;
    float uPrev = 0.0f;

    /// current output, will be solved in NR and then returned as output
    float v;
};