#include "MotorController.h"

static bool motorEnabledState = false;
static MotorDirection motorDirection = MotorDirection::STOPPED;

void initializeMotors()
{
    motorEnabledState = false;
    motorDirection = MotorDirection::STOPPED;
}

void enableMotors()
{
    motorEnabledState = true;
}

void disableMotors()
{
    motorDirection = MotorDirection::STOPPED;
    motorEnabledState = false;
}

bool motorsEnabled()
{
    return motorEnabledState;
}

bool jogMotorForward()
{
    if (!motorEnabledState)
    {
        return false;
    }

    motorDirection = MotorDirection::FORWARD;
    return true;
}

bool jogMotorReverse()
{
    if (!motorEnabledState)
    {
        return false;
    }

    motorDirection = MotorDirection::REVERSE;
    return true;
}

void stopMotor()
{
    motorDirection = MotorDirection::STOPPED;
}

MotorDirection getMotorDirection()
{
    return motorDirection;
}