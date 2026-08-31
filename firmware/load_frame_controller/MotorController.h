#pragma once

#include <Arduino.h>
#include "Types.h"

/*
 * ============================================================
 * MOTOR INITIALIZATION
 * ============================================================
 */

void initializeMotors();

/*
 * ============================================================
 * DRIVER ENABLE / DISABLE
 * ============================================================
 */

void enableMotors();
void disableMotors();

bool motorsEnabled();

/*
 * ============================================================
 * DIRECTION STATE
 * ============================================================
 */

bool jogMotorForward();
bool jogMotorReverse();

void stopMotor();

MotorDirection getMotorDirection();

/*
 * ============================================================
 * BASIC STEP OUTPUT
 * ============================================================
 *
 * Temporary bring-up function.
 *
 * This is intentionally blocking and will later be replaced
 * by deterministic non-blocking step generation.
 */

bool stepMotor(
    uint8_t axis,
    unsigned long steps,
    bool forward
);