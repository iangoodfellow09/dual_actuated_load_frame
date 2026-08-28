#include <Arduino.h>
#include "Communication.h"
#include "Config.h"

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
    else
    {
        Serial.print("ERROR:UNKNOWN_COMMAND=");
        Serial.println(command);
    }
}