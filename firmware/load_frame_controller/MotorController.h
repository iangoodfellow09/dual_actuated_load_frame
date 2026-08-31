#pragma once

#include <Arduino.h>

#include "Types.h"

/*
 * ============================================================
 * MOTOR CONTROLLER INTERFACE
 * ============================================================
 */

void initializeMotors();

void enableMotors();
void disableMotors();

bool motorsEnabled();

bool jogMotorForward();
bool jogMotorReverse();

void stopMotor();

MotorDirection getMotorDirection();

/*
 * Temporary blocking step command.
 */

bool stepMotor(
    uint8_t axis,
    unsigned long steps,
    bool forward
);