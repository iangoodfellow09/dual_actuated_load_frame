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
    /*
     * Set enable pins HIGH before switching them to outputs.
     * HIGH = disabled on the known-good DM542T wiring.
     */

    digitalWrite(
        MOTOR1_EN_PIN,
        MOTOR_DISABLE_LEVEL
    );

    digitalWrite(
        MOTOR2_EN_PIN,
        MOTOR_DISABLE_LEVEL
    );

    pinMode(MOTOR1_EN_PIN, OUTPUT);
    pinMode(MOTOR2_EN_PIN, OUTPUT);

    /*
     * Configure STEP and DIR pins.
     */

    pinMode(MOTOR1_STEP_PIN, OUTPUT);
    pinMode(MOTOR1_DIR_PIN, OUTPUT);

    pinMode(MOTOR2_STEP_PIN, OUTPUT);
    pinMode(MOTOR2_DIR_PIN, OUTPUT);

    /*
     * Safe output states.
     */

    digitalWrite(MOTOR1_STEP_PIN, LOW);
    digitalWrite(MOTOR2_STEP_PIN, LOW);

    digitalWrite(
        MOTOR1_DIR_PIN,
        MOTOR_REVERSE_LEVEL
    );

    digitalWrite(
        MOTOR2_DIR_PIN,
        MOTOR_REVERSE_LEVEL
    );

    /*
     * Explicitly confirm disabled state.
     */

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
    /*
     * Ensure STEP lines are inactive.
     */

    digitalWrite(MOTOR1_STEP_PIN, LOW);
    digitalWrite(MOTOR2_STEP_PIN, LOW);

    /*
     * Disable both DM542T drivers.
     */

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
 * ENABLE STATUS
 * ============================================================
 */

bool motorsEnabled()
{
    return motorEnabledState;
}


/*
 * ============================================================
 * DIRECTION TESTS
 * ============================================================
 */

bool jogMotorForward()
{
    if (!motorEnabledState)
    {
        return false;
    }

    digitalWrite(
        MOTOR1_DIR_PIN,
        MOTOR_FORWARD_LEVEL
    );

    digitalWrite(
        MOTOR2_DIR_PIN,
        MOTOR_FORWARD_LEVEL
    );

    motorDirection = MotorDirection::FORWARD;

    return true;
}


bool jogMotorReverse()
{
    if (!motorEnabledState)
    {
        return false;
    }

    digitalWrite(
        MOTOR1_DIR_PIN,
        MOTOR_REVERSE_LEVEL
    );

    digitalWrite(
        MOTOR2_DIR_PIN,
        MOTOR_REVERSE_LEVEL
    );

    motorDirection = MotorDirection::REVERSE;

    return true;
}


/*
 * ============================================================
 * STOP STATE
 * ============================================================
 *
 * NOTE:
 * This does NOT interrupt an active blocking stepMotor() call.
 * Proper interruptible motion will come later.
 */

void stopMotor()
{
    digitalWrite(MOTOR1_STEP_PIN, LOW);
    digitalWrite(MOTOR2_STEP_PIN, LOW);

    motorDirection = MotorDirection::STOPPED;
}


/*
 * ============================================================
 * DIRECTION STATUS
 * ============================================================
 */

MotorDirection getMotorDirection()
{
    return motorDirection;
}


/*
 * ============================================================
 * BASIC STEP OUTPUT
 * ============================================================
 */

bool stepMotor(
    uint8_t axis,
    unsigned long steps,
    bool forward
)
{
    /*
     * Motion is forbidden while drivers are disabled.
     */

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

    /*
     * Select actuator.
     */

    if (axis == 1)
    {
        stepPin = MOTOR1_STEP_PIN;
        dirPin  = MOTOR1_DIR_PIN;
    }
    else if (axis == 2)
    {
        stepPin = MOTOR2_STEP_PIN;
        dirPin  = MOTOR2_DIR_PIN;
    }
    else
    {
        return false;
    }

    /*
     * Set direction.
     */

    digitalWrite(
        dirPin,
        forward
            ? MOTOR_FORWARD_LEVEL
            : MOTOR_REVERSE_LEVEL
    );

    motorDirection =
        forward
            ? MotorDirection::FORWARD
            : MotorDirection::REVERSE;

    /*
     * Allow direction input to settle before stepping.
     */

    delayMicroseconds(DIR_SETUP_US);

    /*
     * Temporary blocking pulse generator.
     */

    for (
        unsigned long i = 0;
        i < steps;
        ++i
    )
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

    motorDirection = MotorDirection::STOPPED;

    return true;
}