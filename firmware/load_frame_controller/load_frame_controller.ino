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
     * Initialize motor hardware first.
     *
     * This immediately places both DM542T drivers into
     * their disabled state.
     */

    initializeMotors();

    /*
     * Future sensors.
     */

    initializeForceSensors();
    initializePositionSensors();

    /*
     * Future control modules.
     */

    initializeController();
    initializeTrajectory();
    initializeSafety();

    /*
     * Start serial after initialization.
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
    /*
     * Current functional system.
     */

    processSerialCommunication();

    /*
     * Future architecture.
     *
     * These functions currently do nothing.
     */

    updateTrajectory();
    updateController();
}