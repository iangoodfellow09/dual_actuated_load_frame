#include "ForceSensor.h"

/*
 * ============================================================
 * INITIALIZATION
 * ============================================================
 */

void initializeForceSensors()
{
    // Force sensor hardware will be added later.
}

/*
 * ============================================================
 * READ FORCE
 * ============================================================
 */

float readForceNewtons(uint8_t axis)
{
    // Prevent unused parameter warning.
    (void)axis;

    // Placeholder until ADC implementation.
    return 0.0f;
}