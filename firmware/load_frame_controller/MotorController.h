#pragma once

enum class MotorDirection
{
    STOPPED,
    FORWARD,
    REVERSE
};

void initializeMotors();

void enableMotors();
void disableMotors();

bool motorsEnabled();

bool jogMotorForward();
bool jogMotorReverse();

void stopMotor();

MotorDirection getMotorDirection();