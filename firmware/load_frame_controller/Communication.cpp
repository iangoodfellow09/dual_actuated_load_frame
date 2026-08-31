#include <Arduino.h>
#include "Communication.h"
#include "Config.h"
#include "MotorController.h"

void initializeCommunication()
{
    Serial.begin(SERIAL_BAUD_RATE);

    // Give the serial connection a moment to initialize.
    delay(500);

    Serial.println();
    Serial.println(FIRMWARE_NAME);

    Serial.print("FW_VERSION=");
    Serial.println(FIRMWARE_VERSION);

    Serial.println("STATE=IDLE");
    Serial.println("READY");
}

void processSerialCommunication()
{
    if (Serial.available() <= 0)
    {
        return;
    }

    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.length() == 0)
    {
        return;
    }

    if (command == "PING")
    {
        Serial.println("PONG");
    }
    else if (command == "ENABLE")
    {
        enableMotors();
        Serial.println("MOTORS=ENABLED");
    }
    else if (command == "DISABLE")
    {
        disableMotors();
        Serial.println("MOTORS=DISABLED");
    }
    else if (command == "STATUS")
    {
        if (motorsEnabled())
        {
            Serial.println("MOTORS=ENABLED");
        }
        else
        {
            Serial.println("MOTORS=DISABLED");
        }
        MotorDirection direction = getMotorDirection();
        if (direction == MotorDirection::FORWARD)
        {
            Serial.println("MOTOR_DIRECTION=FORWARD");
        }
        else if (direction == MotorDirection::REVERSE)
        {
            Serial.println("MOTOR_DIRECTION=REVERSE");
        }
        else
        {
            Serial.println("MOTOR_DIRECTION=STOPPED");
        }
    }
    else if (command == "JOG_FORWARD")
    {
        if (jogMotorForward())
        {
            Serial.println("MOTOR_DIRECTION=FORWARD");
        }
        else
        {
            Serial.println("ERROR:MOTORS_DISABLED");
        }
    }
    else if (command == "JOG_REVERSE")
    {
        if (jogMotorReverse())
        {
            Serial.println("MOTOR_DIRECTION=REVERSE");
        }
        else
        {
            Serial.println("ERROR:MOTORS_DISABLED");
        }
    }
    else if (command == "STOP")
    {
        stopMotor();
        Serial.println("MOTOR_DIRECTION=STOPPED");
    }

    else
    {
        Serial.print("ERROR:UNKNOWN_COMMAND=");
        Serial.println(command);
    }
}