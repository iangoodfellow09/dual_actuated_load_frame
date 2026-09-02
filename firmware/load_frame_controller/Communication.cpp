#include <Arduino.h>

#include "Communication.h"
#include "Config.h"
#include "MotorController.h"


/*
 * ============================================================
 * PARAMETERIZED STEP COMMAND
 * ============================================================
 *
 * Format:
 *
 * STEP,<axis>,<steps>,<direction>
 *
 * Examples:
 *
 * STEP,1,2000,FWD
 * STEP,1,2000,REV
 * STEP,2,500,FWD
 * STEP,2,500,REV
 */

static void processStepCommand(
    const String &command
)
{
    /*
     * Locate commas.
     */

    int comma1 =
        command.indexOf(',');

    int comma2 =
        command.indexOf(
            ',',
            comma1 + 1
        );

    int comma3 =
        command.indexOf(
            ',',
            comma2 + 1
        );


    /*
     * Verify command format.
     */

    if (
        comma1 < 0 ||
        comma2 < 0 ||
        comma3 < 0
    )
    {
        Serial.println(
            "ERROR:STEP_FORMAT"
        );

        return;
    }


    /*
     * Extract fields.
     */

    String axisString =
        command.substring(
            comma1 + 1,
            comma2
        );

    String stepsString =
        command.substring(
            comma2 + 1,
            comma3
        );

    String directionString =
        command.substring(
            comma3 + 1
        );


    axisString.trim();
    stepsString.trim();
    directionString.trim();


    /*
     * ========================================================
     * AXIS
     * ========================================================
     */

    int axis =
        axisString.toInt();

    if (
        axis != 1 &&
        axis != 2
    )
    {
        Serial.println(
            "ERROR:INVALID_AXIS"
        );

        return;
    }


    /*
     * ========================================================
     * STEP COUNT
     * ========================================================
     */

    long requestedSteps =
        stepsString.toInt();

    if (
        requestedSteps <= 0
    )
    {
        Serial.println(
            "ERROR:INVALID_STEPS"
        );

        return;
    }

    if (
        static_cast<unsigned long>(
            requestedSteps
        )
        > MAX_MANUAL_STEPS
    )
    {
        Serial.println(
            "ERROR:STEP_LIMIT"
        );

        return;
    }


    /*
     * ========================================================
     * DIRECTION
     * ========================================================
     */

    bool forward;

    if (
        directionString == "FWD"
    )
    {
        forward = true;
    }

    else if (
        directionString == "REV"
    )
    {
        forward = false;
    }

    else
    {
        Serial.println(
            "ERROR:INVALID_DIRECTION"
        );

        return;
    }


    /*
     * ========================================================
     * ENABLE CHECK
     * ========================================================
     */

    if (!motorsEnabled())
    {
        Serial.println(
            "ERROR:MOTORS_DISABLED"
        );

        return;
    }


    /*
     * ========================================================
     * EXECUTE MOVE
     * ========================================================
     */

    bool success =
        stepMotor(
            static_cast<uint8_t>(
                axis
            ),
            static_cast<unsigned long>(
                requestedSteps
            ),
            forward
        );

    if (!success)
    {
        Serial.println(
            "ERROR:STEP_FAILED"
        );

        return;
    }


    /*
     * ========================================================
     * CONFIRMATION
     * ========================================================
     */

    Serial.print(
        "OK:STEP,AXIS="
    );

    Serial.print(axis);

    Serial.print(
        ",STEPS="
    );

    Serial.print(
        requestedSteps
    );

    Serial.print(
        ",DIR="
    );

    Serial.println(
        forward
            ? "FWD"
            : "REV"
    );
}


/*
 * ============================================================
 * PARAMETERIZED SPEED COMMAND
 * ============================================================
 *
 * Format:
 *
 * SPEED,<axis>,<steps_per_second>
 *
 * Examples:
 *
 * SPEED,1,500
 * SPEED,1,2000
 * SPEED,2,1000
 */

static void processSpeedCommand(
    const String &command
)
{
    /*
     * Locate commas.
     */

    int comma1 =
        command.indexOf(',');

    int comma2 =
        command.indexOf(
            ',',
            comma1 + 1
        );


    /*
     * Verify command format.
     */

    if (
        comma1 < 0 ||
        comma2 < 0
    )
    {
        Serial.println(
            "ERROR:SPEED_FORMAT"
        );

        return;
    }


    /*
     * Extract fields.
     */

    String axisString =
        command.substring(
            comma1 + 1,
            comma2
        );

    String speedString =
        command.substring(
            comma2 + 1
        );


    axisString.trim();
    speedString.trim();


    /*
     * ========================================================
     * AXIS
     * ========================================================
     */

    int axis =
        axisString.toInt();

    if (
        axis != 1 &&
        axis != 2
    )
    {
        Serial.println(
            "ERROR:INVALID_AXIS"
        );

        return;
    }


    /*
     * ========================================================
     * SPEED
     * ========================================================
     */

    long requestedSpeed =
        speedString.toInt();

    if (
        requestedSpeed <= 0
    )
    {
        Serial.println(
            "ERROR:INVALID_SPEED"
        );

        return;
    }


    /*
     * Check configured limits.
     */

    if (
        requestedSpeed <
            static_cast<long>(
                MIN_STEP_SPS
            )
        ||
        requestedSpeed >
            static_cast<long>(
                MAX_STEP_SPS
            )
    )
    {
        Serial.println(
            "ERROR:SPEED_LIMIT"
        );

        return;
    }


    /*
     * ========================================================
     * STORE SPEED
     * ========================================================
     */

    bool success =
        setMotorStepFrequency(
            static_cast<uint8_t>(
                axis
            ),
            static_cast<unsigned long>(
                requestedSpeed
            )
        );

    if (!success)
    {
        Serial.println(
            "ERROR:SPEED_FAILED"
        );

        return;
    }


    /*
     * ========================================================
     * CONFIRMATION
     * ========================================================
     */

    Serial.print(
        "OK:SPEED,AXIS="
    );

    Serial.print(axis);

    Serial.print(
        ",SPS="
    );

    Serial.println(
        requestedSpeed
    );
}


/*
 * ============================================================
 * INITIALIZE SERIAL COMMUNICATION
 * ============================================================
 */

void initializeCommunication()
{
    Serial.begin(
        SERIAL_BAUD_RATE
    );

    /*
     * Allow serial connection to initialize.
     */

    delay(500);


    /*
     * Startup information.
     */

    Serial.println();

    Serial.println(
        FIRMWARE_NAME
    );

    Serial.print(
        "FW_VERSION="
    );

    Serial.println(
        FIRMWARE_VERSION
    );

    Serial.println(
        "STATE=IDLE"
    );

    Serial.println(
        "READY"
    );
}


/*
 * ============================================================
 * PROCESS SERIAL COMMANDS
 * ============================================================
 */

void processSerialCommunication()
{
    /*
     * No serial data available.
     */

    if (
        Serial.available() <= 0
    )
    {
        return;
    }


    /*
     * Read one command.
     */

    String command =
        Serial.readStringUntil(
            '\n'
        );

    command.trim();


    /*
     * Ignore empty commands.
     */

    if (
        command.length() == 0
    )
    {
        return;
    }


    /*
     * ========================================================
     * PING
     * ========================================================
     */

    if (
        command == "PING"
    )
    {
        Serial.println(
            "PONG"
        );
    }


    /*
     * ========================================================
     * ENABLE
     * ========================================================
     */

    else if (
        command == "ENABLE"
    )
    {
        enableMotors();

        Serial.println(
            "MOTORS=ENABLED"
        );
    }


    /*
     * ========================================================
     * DISABLE
     * ========================================================
     */

    else if (
        command == "DISABLE"
    )
    {
        disableMotors();

        Serial.println(
            "MOTORS=DISABLED"
        );
    }


    /*
     * ========================================================
     * STOP
     * ========================================================
     */

    else if (
        command == "STOP"
    )
    {
        stopMotor();

        Serial.println(
            "MOTOR=STOPPED"
        );
    }


    /*
     * ========================================================
     * JOG FORWARD
     * ========================================================
     *
     * Currently changes direction state only.
     * Continuous jog motion will come later.
     */

    else if (
        command == "JOG_FORWARD"
    )
    {
        if (
            jogMotorForward()
        )
        {
            Serial.println(
                "MOTOR=FORWARD"
            );
        }

        else
        {
            Serial.println(
                "ERROR:MOTORS_DISABLED"
            );
        }
    }


    /*
     * ========================================================
     * JOG REVERSE
     * ========================================================
     */

    else if (
        command == "JOG_REVERSE"
    )
    {
        if (
            jogMotorReverse()
        )
        {
            Serial.println(
                "MOTOR=REVERSE"
            );
        }

        else
        {
            Serial.println(
                "ERROR:MOTORS_DISABLED"
            );
        }
    }


    /*
     * ========================================================
     * STATUS
     * ========================================================
     */

    else if (
        command == "STATUS"
    )
    {
        /*
         * Motor enable state.
         */

        if (
            motorsEnabled()
        )
        {
            Serial.println(
                "MOTORS=ENABLED"
            );
        }

        else
        {
            Serial.println(
                "MOTORS=DISABLED"
            );
        }


        /*
         * Direction state.
         */

        MotorDirection direction =
            getMotorDirection();


        if (
            direction ==
            MotorDirection::FORWARD
        )
        {
            Serial.println(
                "MOTOR=FORWARD"
            );
        }

        else if (
            direction ==
            MotorDirection::REVERSE
        )
        {
            Serial.println(
                "MOTOR=REVERSE"
            );
        }

        else
        {
            Serial.println(
                "MOTOR=STOPPED"
            );
        }


        /*
         * Axis 1 speed.
         */

        Serial.print(
            "AXIS1_SPS="
        );

        Serial.println(
            getMotorStepFrequency(1)
        );


        /*
         * Axis 2 speed.
         */

        Serial.print(
            "AXIS2_SPS="
        );

        Serial.println(
            getMotorStepFrequency(2)
        );
    }


    /*
     * ========================================================
     * STEP COMMAND
     * ========================================================
     */

    else if (
        command.startsWith(
            "STEP,"
        )
    )
    {
        processStepCommand(
            command
        );
    }


    /*
     * ========================================================
     * SPEED COMMAND
     * ========================================================
     */

    else if (
        command.startsWith(
            "SPEED,"
        )
    )
    {
        processSpeedCommand(
            command
        );
    }


    /*
     * ========================================================
     * UNKNOWN COMMAND
     * ========================================================
     */

    else
    {
        Serial.print(
            "ERROR:UNKNOWN_COMMAND="
        );

        Serial.println(
            command
        );
    }
}