#pragma once

#include <Arduino.h>

/*
 * ============================================================
 * FORCE SENSOR INTERFACE
 * ============================================================
 *
 * Placeholder for future ADC / load-cell implementation.
 */

void initializeForceSensors();

float readForceNewtons(uint8_t axis);