#pragma once

/*
 * ============================================================
 * SAFETY SYSTEM
 * ============================================================
 *
 * Future home of:
 *
 * - Force limits
 * - Travel limits
 * - Limit switches
 * - Watchdog
 * - Sensor fault detection
 */

void initializeSafety();

bool safetyFaultActive();