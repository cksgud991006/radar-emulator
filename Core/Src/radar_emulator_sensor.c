/*
 * sensor.c
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#include <radar_emulator_sensor.h>
#include "VL53L1X_api.h"

uint16_t GetMaxRange(uint16_t mode) {

	uint16_t distanceMm = 0;

	switch(mode) {
		// short distance mode
		case 1: distanceMm = 1300; // ~1.3m
		// long distance mode
		case 2: distanceMm = 4000; // ~4.0m

		default: distanceMm = 1300; // ~1.3m
	}

	return distanceMm;
}

bool IsTargetValid(TargetData* targetData) {

	// for short mode, ranging distance ensured is 40 mm ~ 1300 mm.
	// for long mode, ranging distance ensured is 40 mm ~ 4000 mm.
	VL53L1X_ERROR status;

	uint16_t distanceMm = targetData->distanceMm;
	// 1. timeout / no target
	if (distanceMm == 0xffff) return false;

	uint16_t mode;
	status = VL53L1X_GetDistanceMode(VL53L1X_ADDRESS, &mode);

	uint16_t minRangeMm = 0;
	uint16_t maxRangeMm = 0;

	maxRangeMm = GetMaxRange(mode);

	// 2. mode-based validation
	if (minRangeMm < distanceMm && distanceMm < maxRangeMm) return true;

	return false;

}
