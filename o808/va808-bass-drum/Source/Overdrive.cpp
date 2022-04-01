/*
  ==============================================================================

    "Overdrive.h"
    Part of: Roland TR-808 Virtual Analogue Modelling - MSc Project
    Created: 21th July 2021
    Author:  Cameron Smith, UoE s1338237

    ----------------------see header file for description-----------------------
  ==============================================================================
*/

#include "Overdrive.h"
#include <math.h>

void Overdrive::setSampleRate(float SR)
{
    float timeStep = 1.0f / SR;

    // calculation of some state-space values
    A = -1.0f / (resistance * capacitance);
    B = 1.0f / (resistance * capacitance);
    C = -1.0f / capacitance;

    // these are then used for deriving further state-space parameters
    Ha = 1.0f / (2.0f / timeStep - A);
    Hb = 2.0f / timeStep + A;
    K = D * Ha * C + F;
    G = 2.0f * satCurrent / (diodeIdealityFac * diodeThermalVoltage);

    // calculating an initial p[n] value with zero input to calculate linear approx of v as initial guess 
    float p = D * Ha * (Hb * xPrev + B * uPrev + C * iPrev);
    v = p / (1.0f - K * G);
}

float Overdrive::process(float input)
{
    // calculating p[n] at current input
    float p = D * Ha * (Hb * xPrev + B * uPrev + C * iPrev) + (D * Ha * B + E) * input;
    
    // reset NR difference and iteration counter
    float eps = 1.0f;
    int iter = 0;

    // NEWTON-RAPHSON SOLVING
    while (eps > tolerance && maxIterations > iter)
    {
        // calculating function value at v_i
        float g = p + K * satCurrent * (expf(v / (2.0f * diodeIdealityFac * diodeThermalVoltage)) - expf(-v / (diodeIdealityFac * diodeThermalVoltage))) - v;

        // calculating Jacobian value at v_i
        float J = K * satCurrent / (diodeIdealityFac * diodeThermalVoltage) * (0.5f * expf(v / (2.0f * diodeIdealityFac * diodeThermalVoltage)) + expf(-v / (diodeIdealityFac * diodeThermalVoltage))) - 1.0f;

        // calculate next iteration, new v value
        float vnew = v - (g / J);

        // difference between new and previous iteration
        eps = fabsf(vnew - v);

        // update previous iteration and move up an iteration
        v = vnew;
        iter++;
    }

    //current through diodes
    float i0 = satCurrent * (expf(v / (2.0f * diodeIdealityFac * diodeThermalVoltage)) - expf(-v / (diodeIdealityFac * diodeThermalVoltage)));
    
    //state update
    float x0 = Ha * (Hb * xPrev + B * (input + uPrev) + C * (i0 + iPrev));

    // update previous variables
    iPrev = i0;
    uPrev = input;
    xPrev = x0;

    // returning outut of asymmetric diode clipper circuit
    return v;

}
