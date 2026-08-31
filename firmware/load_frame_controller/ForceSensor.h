#pragma once

#include <Arduino.h>

/*
 * ============================================================
 * FORCE SENSOR INTERFACE
 * ============================================================
 *
 * Placeholder until load-cell ADC integration.
 */

void initializeForceSensors();

float readForceNewtons(
    uint8_t axis
);