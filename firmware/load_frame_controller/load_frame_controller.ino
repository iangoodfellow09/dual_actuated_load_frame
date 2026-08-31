#include "Communication.h"
#include "MotorController.h"
#include "ForceSensor.h"
#include "PositionSensor.h"
#include "Controller.h"
#include "Trajectory.h"
#include "Safety.h"

/*
 * ============================================================
 * SETUP
 * ============================================================
 */

void setup()
{
    /*
     * Motor initialization comes first because it immediately
     * places the DM542T drivers into the disabled state.
     */

    initializeMotors();

    initializeForceSensors();
    initializePositionSensors();

    initializeController();
    initializeTrajectory();
    initializeSafety();

    /*
     * Serial comes last so READY means initialization has
     * completed.
     */

    initializeCommunication();
}

/*
 * ============================================================
 * MAIN LOOP
 * ============================================================
 */

void loop()
{
    processSerialCommunication();

    /*
     * These currently do nothing, but this preserves the
     * architecture for later development.
     */

    updateTrajectory();
    updateController();
}