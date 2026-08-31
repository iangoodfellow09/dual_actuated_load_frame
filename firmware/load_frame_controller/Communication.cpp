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
     * Locate command separators.
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
     * Extract parameters.
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
     * Axis
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
     * Steps
     */

    long requestedSteps =
        stepsString.toInt();

    if (requestedSteps <= 0)
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
     * Direction
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
     * Motor must already be enabled.
     */

    if (!motorsEnabled())
    {
        Serial.println(
            "ERROR:MOTORS_DISABLED"
        );

        return;
    }

    /*
     * Execute move.
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
     * Confirmation.
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
 * INITIALIZATION
 * ============================================================
 */

void initializeCommunication()
{
    Serial.begin(
        SERIAL_BAUD_RATE
    );

    delay(500);

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
 * SERIAL COMMAND PROCESSING
 * ============================================================
 */

void processSerialCommunication()
{
    if (
        Serial.available() <= 0
    )
    {
        return;
    }

    String command =
        Serial.readStringUntil(
            '\n'
        );

    command.trim();

    if (
        command.length() == 0
    )
    {
        return;
    }


    /*
     * --------------------------------------------------------
     * PING
     * --------------------------------------------------------
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
     * --------------------------------------------------------
     * ENABLE
     * --------------------------------------------------------
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
     * --------------------------------------------------------
     * DISABLE
     * --------------------------------------------------------
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
     * --------------------------------------------------------
     * STOP
     * --------------------------------------------------------
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
     * --------------------------------------------------------
     * DIRECTION TESTS
     * --------------------------------------------------------
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
     * --------------------------------------------------------
     * STATUS
     * --------------------------------------------------------
     */

    else if (
        command == "STATUS"
    )
    {
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
    }


    /*
     * --------------------------------------------------------
     * STEP
     * --------------------------------------------------------
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
     * --------------------------------------------------------
     * UNKNOWN COMMAND
     * --------------------------------------------------------
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