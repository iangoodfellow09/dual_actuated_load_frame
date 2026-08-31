#include <Arduino.h>

#include "MotorController.h"
#include "Config.h"

/*
 * ============================================================
 * INTERNAL STATE
 * ============================================================
 */

static bool motorEnabledState = false;

static MotorDirection motorDirection =
    MotorDirection::STOPPED;

/*
 * ============================================================
 * INITIALIZATION
 * ============================================================
 */

void initializeMotors()
{
    // Axis 1
    pinMode(MOTOR1_STEP_PIN, OUTPUT);
    pinMode(MOTOR1_DIR_PIN, OUTPUT);
    pinMode(MOTOR1_EN_PIN, OUTPUT);

    // Axis 2
    pinMode(MOTOR2_STEP_PIN, OUTPUT);
    pinMode(MOTOR2_DIR_PIN, OUTPUT);
    pinMode(MOTOR2_EN_PIN, OUTPUT);

    // Step outputs inactive
    digitalWrite(MOTOR1_STEP_PIN, LOW);
    digitalWrite(MOTOR2_STEP_PIN, LOW);

    // Initial direction state
    digitalWrite(MOTOR1_DIR_PIN, LOW);
    digitalWrite(MOTOR2_DIR_PIN, LOW);

    // IMPORTANT:
    // Drivers always start disabled.
    digitalWrite(
        MOTOR1_EN_PIN,
        MOTOR_DISABLE_LEVEL
    );

    digitalWrite(
        MOTOR2_EN_PIN,
        MOTOR_DISABLE_LEVEL
    );

    motorEnabledState = false;
    motorDirection = MotorDirection::STOPPED;
}

/*
 * ============================================================
 * ENABLE
 * ============================================================
 */

void enableMotors()
{
    digitalWrite(
        MOTOR1_EN_PIN,
        MOTOR_ENABLE_LEVEL
    );

    digitalWrite(
        MOTOR2_EN_PIN,
        MOTOR_ENABLE_LEVEL
    );

    motorEnabledState = true;
}

/*
 * ============================================================
 * DISABLE
 * ============================================================
 */

void disableMotors()
{
    // Ensure no step output remains high
    digitalWrite(MOTOR1_STEP_PIN, LOW);
    digitalWrite(MOTOR2_STEP_PIN, LOW);

    digitalWrite(
        MOTOR1_EN_PIN,
        MOTOR_DISABLE_LEVEL
    );

    digitalWrite(
        MOTOR2_EN_PIN,
        MOTOR_DISABLE_LEVEL
    );

    motorDirection = MotorDirection::STOPPED;
    motorEnabledState = false;
}

/*
 * ============================================================
 * ENABLE STATE
 * ============================================================
 */

bool motorsEnabled()
{
    return motorEnabledState;
}

/*
 * ============================================================
 * FORWARD DIRECTION STATE
 * ============================================================
 */

bool jogMotorForward()
{
    if (!motorEnabledState)
    {
        return false;
    }

    digitalWrite(MOTOR1_DIR_PIN, HIGH);
    digitalWrite(MOTOR2_DIR_PIN, HIGH);

    motorDirection = MotorDirection::FORWARD;

    return true;
}

/*
 * ============================================================
 * REVERSE DIRECTION STATE
 * ============================================================
 */

bool jogMotorReverse()
{
    if (!motorEnabledState)
    {
        return false;
    }

    digitalWrite(MOTOR1_DIR_PIN, LOW);
    digitalWrite(MOTOR2_DIR_PIN, LOW);

    motorDirection = MotorDirection::REVERSE;

    return true;
}

/*
 * ============================================================
 * STOP
 * ============================================================
 */

void stopMotor()
{
    digitalWrite(MOTOR1_STEP_PIN, LOW);
    digitalWrite(MOTOR2_STEP_PIN, LOW);

    motorDirection = MotorDirection::STOPPED;
}

/*
 * ============================================================
 * GET DIRECTION
 * ============================================================
 */

MotorDirection getMotorDirection()
{
    return motorDirection;
}

/*
 * ============================================================
 * BASIC STEP MOTOR TEST
 * ============================================================
 */

bool stepMotor(
    uint8_t axis,
    unsigned long steps,
    bool forward
)
{
    if (!motorEnabledState)
    {
        return false;
    }

    if (steps == 0)
    {
        return false;
    }

    uint8_t stepPin;
    uint8_t dirPin;

    if (axis == 1)
    {
        stepPin = MOTOR1_STEP_PIN;
        dirPin = MOTOR1_DIR_PIN;
    }
    else if (axis == 2)
    {
        stepPin = MOTOR2_STEP_PIN;
        dirPin = MOTOR2_DIR_PIN;
    }
    else
    {
        return false;
    }

    /*
     * Set direction before generating step pulses.
     */

    digitalWrite(
        dirPin,
        forward ? HIGH : LOW
    );

    motorDirection =
        forward
            ? MotorDirection::FORWARD
            : MotorDirection::REVERSE;

    delayMicroseconds(DIR_SETUP_US);

    /*
     * Temporary blocking step generator.
     */

    for (unsigned long i = 0; i < steps; ++i)
    {
        digitalWrite(stepPin, HIGH);

        delayMicroseconds(
            STEP_HIGH_US
        );

        digitalWrite(stepPin, LOW);

        delayMicroseconds(
            STEP_LOW_US
        );
    }

    /*
     * Command has completed.
     */

    motorDirection = MotorDirection::STOPPED;

    return true;
}