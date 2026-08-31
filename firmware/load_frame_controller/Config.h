#pragma once

#include <Arduino.h>

/*
 * ============================================================
 * FIRMWARE INFORMATION
 * ============================================================
 */

constexpr char FIRMWARE_NAME[] = "DUAL_ACTUATOR_LOAD_FRAME";
constexpr char FIRMWARE_VERSION[] = "0.1.0";

/*
 * ============================================================
 * SERIAL COMMUNICATION
 * ============================================================
 */

constexpr unsigned long SERIAL_BAUD_RATE = 115200;

/*
 * ============================================================
 * AXIS 1 - DM542T
 * ============================================================
 */

constexpr uint8_t MOTOR1_STEP_PIN = 8;
constexpr uint8_t MOTOR1_DIR_PIN  = 9;
constexpr uint8_t MOTOR1_EN_PIN   = 10;

/*
 * ============================================================
 * AXIS 2 - DM542T
 * ============================================================
 */

constexpr uint8_t MOTOR2_STEP_PIN = 11;
constexpr uint8_t MOTOR2_DIR_PIN  = 12;
constexpr uint8_t MOTOR2_EN_PIN   = 13;

/*
 * ============================================================
 * DM542T ENABLE LOGIC
 * ============================================================
 *
 * Known working common-anode configuration:
 *
 * LOW  = enabled
 * HIGH = disabled
 */

constexpr uint8_t MOTOR_ENABLE_LEVEL  = LOW;
constexpr uint8_t MOTOR_DISABLE_LEVEL = HIGH;

/*
 * ============================================================
 * INITIAL STEP TIMING
 * ============================================================
 *
 * Conservative values for hardware bring-up.
 * These are NOT the final frequency-control implementation.
 */

constexpr unsigned long DIR_SETUP_US = 10;
constexpr unsigned long STEP_HIGH_US = 5;
constexpr unsigned long STEP_LOW_US  = 1000;