#pragma once

#include <Arduino.h>

/*
 * ============================================================
 * POSITION SENSOR INTERFACE
 * ============================================================
 *
 * Placeholder until displacement feedback integration.
 */

void initializePositionSensors();

float readPositionMillimeters(
    uint8_t axis
);