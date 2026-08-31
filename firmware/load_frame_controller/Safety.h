#pragma once

/*
 * ============================================================
 * SOFTWARE SAFETY SYSTEM
 * ============================================================
 *
 * Future:
 *
 * force limits
 * displacement limits
 * limit switches
 * watchdog
 * sensor failure detection
 */

void initializeSafety();

bool safetyFaultActive();