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
 * AXIS STEP FREQUENCY STATE
 * ============================================================
 */

static unsigned long motor1StepSPS =
    DEFAULT_STEP_SPS;

static unsigned long motor2StepSPS =
    DEFAULT_STEP_SPS;


/*
 * ============================================================
 * INITIALIZATION
 * ============================================================
 */

void initializeMotors()
{
    /*
     * Put enable outputs into the disabled state before
     * configuring them as outputs.
     */

    digitalWrite(
        MOTOR1_EN_PIN,
        MOTOR_DISABLE_LEVEL
    );

    digitalWrite(
        MOTOR2_EN_PIN,
        MOTOR_DISABLE_LEVEL
    );

    /*
     * Configure enable pins.
     */

    pinMode(
        MOTOR1_EN_PIN,
        OUTPUT
    );

    pinMode(
        MOTOR2_EN_PIN,
        OUTPUT
    );

    /*
     * Configure STEP and DIR pins.
     */

    pinMode(
        MOTOR1_STEP_PIN,
        OUTPUT
    );

    pinMode(
        MOTOR1_DIR_PIN,
        OUTPUT
    );

    pinMode(
        MOTOR2_STEP_PIN,
        OUTPUT
    );

    pinMode(
        MOTOR2_DIR_PIN,
        OUTPUT
    );

    /*
     * STEP outputs inactive.
     */

    digitalWrite(
        MOTOR1_STEP_PIN,
        LOW
    );

    digitalWrite(
        MOTOR2_STEP_PIN,
        LOW
    );

    /*
     * Set known direction states.
     */

    digitalWrite(
        MOTOR1_DIR_PIN,
        MOTOR_REVERSE_LEVEL
    );

    digitalWrite(
        MOTOR2_DIR_PIN,
        MOTOR_REVERSE_LEVEL
    );

    /*
     * Explicitly confirm both drivers are disabled.
     */

    digitalWrite(
        MOTOR1_EN_PIN,
        MOTOR_DISABLE_LEVEL
    );

    digitalWrite(
        MOTOR2_EN_PIN,
        MOTOR_DISABLE_LEVEL
    );

    /*
     * Reset software state.
     */

    motorEnabledState = false;

    motorDirection =
        MotorDirection::STOPPED;

    /*
     * Reset both axes to default speed.
     */

    motor1StepSPS =
        DEFAULT_STEP_SPS;

    motor2StepSPS =
        DEFAULT_STEP_SPS;
}


/*
 * ============================================================
 * ENABLE MOTORS
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
 * DISABLE MOTORS
 * ============================================================
 */

void disableMotors()
{
    /*
     * Ensure STEP outputs are inactive.
     */

    digitalWrite(
        MOTOR1_STEP_PIN,
        LOW
    );

    digitalWrite(
        MOTOR2_STEP_PIN,
        LOW
    );

    /*
     * Disable both motor drivers.
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

    motorDirection =
        MotorDirection::STOPPED;
}


/*
 * ============================================================
 * MOTOR ENABLE STATUS
 * ============================================================
 */

bool motorsEnabled()
{
    return motorEnabledState;
}


/*
 * ============================================================
 * JOG FORWARD STATE
 * ============================================================
 *
 * This currently changes direction state only.
 * It does not generate continuous motion.
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

    motorDirection =
        MotorDirection::FORWARD;

    return true;
}


/*
 * ============================================================
 * JOG REVERSE STATE
 * ============================================================
 */

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

    motorDirection =
        MotorDirection::REVERSE;

    return true;
}


/*
 * ============================================================
 * STOP MOTOR STATE
 * ============================================================
 *
 * NOTE:
 * This cannot interrupt an active blocking stepMotor() call.
 * Non-blocking motion control will be implemented later.
 */

void stopMotor()
{
    digitalWrite(
        MOTOR1_STEP_PIN,
        LOW
    );

    digitalWrite(
        MOTOR2_STEP_PIN,
        LOW
    );

    motorDirection =
        MotorDirection::STOPPED;
}


/*
 * ============================================================
 * GET CURRENT DIRECTION STATE
 * ============================================================
 */

MotorDirection getMotorDirection()
{
    return motorDirection;
}


/*
 * ============================================================
 * SET STEP FREQUENCY
 * ============================================================
 */

bool setMotorStepFrequency(
    uint8_t axis,
    unsigned long stepsPerSecond
)
{
    /*
     * Reject speeds outside configured limits.
     */

    if (
        stepsPerSecond < MIN_STEP_SPS ||
        stepsPerSecond > MAX_STEP_SPS
    )
    {
        return false;
    }

    /*
     * Axis 1.
     */

    if (axis == 1)
    {
        motor1StepSPS =
            stepsPerSecond;

        return true;
    }

    /*
     * Axis 2.
     */

    if (axis == 2)
    {
        motor2StepSPS =
            stepsPerSecond;

        return true;
    }

    /*
     * Invalid axis.
     */

    return false;
}


/*
 * ============================================================
 * GET STEP FREQUENCY
 * ============================================================
 */

unsigned long getMotorStepFrequency(
    uint8_t axis
)
{
    if (axis == 1)
    {
        return motor1StepSPS;
    }

    if (axis == 2)
    {
        return motor2StepSPS;
    }

    /*
     * Invalid axis.
     */

    return 0;
}


/*
 * ============================================================
 * BLOCKING MICROSECOND DELAY HELPER
 * ============================================================
 *
 * Long delays are split into milliseconds plus the remaining
 * microseconds.
 *
 * This is temporary and will eventually be replaced by
 * non-blocking/timer-based motion control.
 */

static void waitMicroseconds(
    unsigned long microseconds
)
{
    while (
        microseconds >= 1000UL
    )
    {
        delay(1);

        microseconds -=
            1000UL;
    }

    if (microseconds > 0)
    {
        delayMicroseconds(
            static_cast<unsigned int>(
                microseconds
            )
        );
    }
}


/*
 * ============================================================
 * BASIC STEP OUTPUT
 * ============================================================
 *
 * This is currently a blocking step generator.
 *
 * The selected axis:
 *
 * 1. Uses its configured SPS value
 * 2. Sets DIR
 * 3. Generates the requested number of STEP pulses
 * 4. Returns when the movement is complete
 */

bool stepMotor(
    uint8_t axis,
    unsigned long steps,
    bool forward
)
{
    /*
     * Motors must already be enabled.
     */

    if (!motorEnabledState)
    {
        return false;
    }

    /*
     * Reject zero-step commands.
     */

    if (steps == 0)
    {
        return false;
    }

    uint8_t stepPin;
    uint8_t dirPin;

    /*
     * ========================================================
     * SELECT AXIS
     * ========================================================
     */

    if (axis == 1)
    {
        stepPin =
            MOTOR1_STEP_PIN;

        dirPin =
            MOTOR1_DIR_PIN;
    }

    else if (axis == 2)
    {
        stepPin =
            MOTOR2_STEP_PIN;

        dirPin =
            MOTOR2_DIR_PIN;
    }

    else
    {
        return false;
    }


    /*
     * ========================================================
     * GET AXIS STEP FREQUENCY
     * ========================================================
     */

    unsigned long stepsPerSecond =
        getMotorStepFrequency(
            axis
        );

    if (
        stepsPerSecond == 0
    )
    {
        return false;
    }


    /*
     * ========================================================
     * CONVERT SPS TO STEP PERIOD
     * ========================================================
     *
     * Examples:
     *
     * 500 SPS  = 2000 us per step
     * 1000 SPS = 1000 us per step
     * 2000 SPS = 500 us per step
     * 5000 SPS = 200 us per step
     */

    unsigned long stepPeriodUs =
        1000000UL /
        stepsPerSecond;


    /*
     * STEP_HIGH_US occupies part of the full period.
     */

    unsigned long stepLowTimeUs;

    if (
        stepPeriodUs >
        STEP_HIGH_US
    )
    {
        stepLowTimeUs =
            stepPeriodUs -
            STEP_HIGH_US;
    }

    else
    {
        stepLowTimeUs = 1;
    }


    /*
     * ========================================================
     * SET DIRECTION
     * ========================================================
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
     * Give the DM542T direction signal time to settle before
     * the first STEP pulse.
     */

    delayMicroseconds(
        DIR_SETUP_US
    );


    /*
     * ========================================================
     * GENERATE STEP PULSES
     * ========================================================
     */

    for (
        unsigned long i = 0;
        i < steps;
        ++i
    )
    {
        /*
         * STEP high.
         */

        digitalWrite(
            stepPin,
            HIGH
        );

        delayMicroseconds(
            STEP_HIGH_US
        );

        /*
         * STEP low.
         */

        digitalWrite(
            stepPin,
            LOW
        );

        /*
         * Wait for the remainder of the step period.
         */

        waitMicroseconds(
            stepLowTimeUs
        );
    }


    /*
     * ========================================================
     * MOVE COMPLETE
     * ========================================================
     */

    motorDirection =
        MotorDirection::STOPPED;

    return true;
}