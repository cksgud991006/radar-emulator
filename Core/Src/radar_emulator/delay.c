/*
 * delay.c
 *
 *  Created on: Apr 27, 2026
 *      Author: Chanhyung Shim
 */


#include <radar_emulator/delay.h>
#include <radar_emulator/angle.h>
#include <radar_emulator/config.h>
#include <stdlib.h>

uint32_t CalculateServoDelay(int16_t stepAngleDeg) {
	// servo operates transit speed of 2 ms per degree
	uint32_t ticksMs = abs(stepAngleDeg * 2);

	return ticksMs;
}

uint32_t CalculateTaskDelay(int16_t angleDeg, uint16_t steps) {

	int16_t minRangeDeg, maxRangeDeg;

	GetTrackRange(angleDeg, &minRangeDeg, &maxRangeDeg);

	int16_t stepAngleDeg = CalculateStepAngle(minRangeDeg, maxRangeDeg, steps);

	uint32_t delay = CalculateServoDelay(stepAngleDeg) * steps;

	return delay * MOTION_SMOOTHING_FACTOR;

}
