#include "Communication.h"
#include "MotorController.h"

void setup()
{
    initializeMotors();
    initializeCommunication();
}

void loop()
{
    processSerialCommunication();
}