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
 * ============================================================
 * STEP FREQUENCY CONTROL
 * ============================================================
 */

bool setMotorStepFrequency(
    uint8_t axis,
    unsigned long stepsPerSecond
);

unsigned long getMotorStepFrequency(
    uint8_t axis
);

/*
 * Temporary blocking step command.
 */

bool stepMotor(
    uint8_t axis,
    unsigned long steps,
    bool forward
);