#include <Arduino.h>
#include "MotorController.h"
#include "Config.h"

static bool motorEnabledState = false;
static MotorDirection motorDirection = MotorDirection::STOPPED;

void initializeMotors()
{
    pinMode(MOTOR1_STEP_PIN, OUTPUT);
    pinMode(MOTOR1_DIR_PIN, OUTPUT);
    pinMode(MOTOR1_EN_PIN, OUTPUT);

    pinMode(MOTOR2_STEP_PIN, OUTPUT);
    pinMode(MOTOR2_DIR_PIN, OUTPUT);
    pinMode(MOTOR2_EN_PIN, OUTPUT);

    digitalWrite(MOTOR1_STEP_PIN, LOW);
    digitalWrite(MOTOR2_STEP_PIN, LOW);

    // Safe startup: drivers disabled
    digitalWrite(MOTOR1_EN_PIN, MOTOR_DISABLE_LEVEL);
    digitalWrite(MOTOR2_EN_PIN, MOTOR_DISABLE_LEVEL);

    motorEnabledState = false;
    motorDirection = MotorDirection::STOPPED;
}

void enableMotors()
{
    digitalWrite(MOTOR1_EN_PIN, MOTOR_ENABLE_LEVEL);
    digitalWrite(MOTOR2_EN_PIN, MOTOR_ENABLE_LEVEL);

    motorEnabledState = true;
}

void disableMotors()
{
    digitalWrite(MOTOR1_EN_PIN, MOTOR_DISABLE_LEVEL);
    digitalWrite(MOTOR2_EN_PIN, MOTOR_DISABLE_LEVEL);

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