#pragma once

#include <Arduino.h>

/*
 * ============================================================
 * FIRMWARE
 * ============================================================
 */

constexpr char FIRMWARE_NAME[] = "DUAL_ACTUATOR_LOAD_FRAME";
constexpr char FIRMWARE_VERSION[] = "0.2.0";

/*
 * ============================================================
 * SERIAL
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
 * DM542T LOGIC
 * ============================================================
 *
 * Known-good common-anode configuration.
 */

constexpr uint8_t MOTOR_ENABLE_LEVEL  = LOW;
constexpr uint8_t MOTOR_DISABLE_LEVEL = HIGH;

constexpr uint8_t MOTOR_FORWARD_LEVEL = HIGH;
constexpr uint8_t MOTOR_REVERSE_LEVEL = LOW;

/*
 * ============================================================
 * BASIC STEP TIMING
 * ============================================================
 *
 * Temporary conservative blocking step generation.
 */

constexpr unsigned long DIR_SETUP_US = 10;
constexpr unsigned long STEP_HIGH_US = 5;
constexpr unsigned long STEP_LOW_US  = 1000;

/*
 * ============================================================
 * MANUAL MOTION LIMIT
 * ============================================================
 */

constexpr unsigned long MAX_MANUAL_STEPS = 20000;

/*
 * ============================================================
 * STEP FREQUENCY SETTINGS
 * ============================================================
 *
 * Speed is expressed in steps per second (SPS).
 *
 * These are conservative limits for the current blocking
 * step generator. We can increase them later when we move to
 * timer-based/non-blocking pulse generation.
 */

constexpr unsigned long DEFAULT_STEP_SPS = 1000;

constexpr unsigned long MIN_STEP_SPS = 50;

constexpr unsigned long MAX_STEP_SPS = 5000;