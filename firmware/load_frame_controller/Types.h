#pragma once

#include <stdint.h>

/*
 * ============================================================
 * SHARED TYPES
 * ============================================================
 */

enum class AxisId : uint8_t
{
    AXIS_1 = 1,
    AXIS_2 = 2
};

enum class MotorDirection : uint8_t
{
    STOPPED,
    FORWARD,
    REVERSE
};