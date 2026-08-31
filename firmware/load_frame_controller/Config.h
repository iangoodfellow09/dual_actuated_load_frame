#pragma once

constexpr unsigned long SERIAL_BAUD_RATE = 115200;

constexpr char FIRMWARE_NAME[] = "DUAL_ACTUATOR_LOAD_FRAME";
constexpr char FIRMWARE_VERSION[] = "0.1.0";

// Axis 1 - DM542T
constexpr uint8_t MOTOR1_STEP_PIN = 8;
constexpr uint8_t MOTOR1_DIR_PIN  = 9;
constexpr uint8_t MOTOR1_EN_PIN   = 10;

// Axis 2 - DM542T
constexpr uint8_t MOTOR2_STEP_PIN = 11;
constexpr uint8_t MOTOR2_DIR_PIN  = 12;
constexpr uint8_t MOTOR2_EN_PIN   = 13;

// Common-anode DM542T wiring
constexpr uint8_t MOTOR_ENABLE_LEVEL  = LOW;
constexpr uint8_t MOTOR_DISABLE_LEVEL = HIGH;