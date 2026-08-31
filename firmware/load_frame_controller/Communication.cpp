#include <Arduino.h>

#include "Communication.h"
#include "Config.h"
#include "MotorController.h"

/*
 * ============================================================
 * INITIALIZATION
 * ============================================================
 */

void initializeCommunication()
{
    Serial.begin(SERIAL_BAUD_RATE);

    delay(500);

    Serial.println();
    Serial.println(FIRMWARE_NAME);

    Serial.print("FW_VERSION=");
    Serial.println(FIRMWARE_VERSION);

    Serial.println("STATE=IDLE");
    Serial.println("READY");
}

/*
 * ============================================================
 * SERIAL COMMAND PROCESSING
 * ============================================================
 */

void processSerialCommunication()
{
    if (Serial.available() <= 0)
    {
        return;
    }

    String command =
        Serial.readStringUntil('\n');

    command.trim();

    if (command.length() == 0)
    {
        return;
    }

    /*
     * --------------------------------------------------------
     * CONNECTION TEST
     * --------------------------------------------------------
     */

    if (command == "PING")
    {
        Serial.println("PONG");
    }

    /*
     * --------------------------------------------------------
     * ENABLE
     * --------------------------------------------------------
     */

    else if (command == "ENABLE")
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

    else if (command == "DISABLE")
    {
        disableMotors();

        Serial.println(
            "MOTORS=DISABLED"
        );
    }

    /*
     * --------------------------------------------------------
     * SOFTWARE DIRECTION TEST
     * --------------------------------------------------------
     */

    else if (command == "JOG_FORWARD")
    {
        if (jogMotorForward())
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

    else if (command == "JOG_REVERSE")
    {
        if (jogMotorReverse())
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
     * STOP
     * --------------------------------------------------------
     */

    else if (command == "STOP")
    {
        stopMotor();

        Serial.println(
            "MOTOR=STOPPED"
        );
    }

    /*
     * --------------------------------------------------------
     * STATUS
     * --------------------------------------------------------
     */

    else if (command == "STATUS")
    {
        if (motorsEnabled())
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
     * AXIS 1 STEP TEST
     * --------------------------------------------------------
     */

    else if (
        command ==
        "TEST_STEP_1_FORWARD"
    )
    {
        if (
            stepMotor(
                1,
                10000,
                true
            )
        )
        {
            Serial.println(
                "OK:AXIS1_FORWARD"
            );
        }
        else
        {
            Serial.println(
                "ERROR:STEP_FAILED"
            );
        }
    }

    else if (
        command ==
        "TEST_STEP_1_REVERSE"
    )
    {
        if (
            stepMotor(
                1,
                100,
                false
            )
        )
        {
            Serial.println(
                "OK:AXIS1_REVERSE"
            );
        }
        else
        {
            Serial.println(
                "ERROR:STEP_FAILED"
            );
        }
    }

    /*
     * --------------------------------------------------------
     * AXIS 2 STEP TEST
     * --------------------------------------------------------
     */

    else if (
        command ==
        "TEST_STEP_2_FORWARD"
    )
    {
        if (
            stepMotor(
                2,
                100,
                true
            )
        )
        {
            Serial.println(
                "OK:AXIS2_FORWARD"
            );
        }
        else
        {
            Serial.println(
                "ERROR:STEP_FAILED"
            );
        }
    }

    else if (
        command ==
        "TEST_STEP_2_REVERSE"
    )
    {
        if (
            stepMotor(
                2,
                100,
                false
            )
        )
        {
            Serial.println(
                "OK:AXIS2_REVERSE"
            );
        }
        else
        {
            Serial.println(
                "ERROR:STEP_FAILED"
            );
        }
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

        Serial.println(command);
    }
}