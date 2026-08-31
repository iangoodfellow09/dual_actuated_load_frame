#pragma once

#include <Arduino.h>

/*
 * ============================================================
 * POSITION SENSOR INTERFACE
 * ============================================================
 *
 * Placeholder for future displacement feedback.
 */

void initializePositionSensors();

float readPositionMillimeters(uint8_t axis);