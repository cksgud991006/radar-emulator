/*
 * angle.c
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#include <stdlib.h>
#include <math.h>
#include "radar_emulator/angle.h"
#include "radar_emulator/math.h"

int16_t CalculateStepAngle(int16_t angleL1Deg, int16_t angleL2Deg, uint32_t numTicks) {

	uint16_t angleDeg = abs(angleL1Deg - angleL2Deg);

	if (fmod((double) angleDeg, (double) numTicks) != 0) return 0;

	// TODO: find valid step angle
	return angleDeg / numTicks;

}

uint16_t SetNextAngle(int16_t* anglePtr, int16_t* stepAngleDegPtr, int16_t minServoAngleDeg, int16_t maxServoAngleDeg) {
	// set the range limit and switch angle direction when angle moves beyond the limit

	if (abs(*stepAngleDegPtr) > abs(maxServoAngleDeg - minServoAngleDeg)) return 1;

	int16_t angleDeg = *anglePtr;

	angleDeg += *stepAngleDegPtr;

	if (angleDeg < minServoAngleDeg) {

		uint16_t diff = abs(angleDeg - minServoAngleDeg);
		angleDeg = max(minServoAngleDeg, angleDeg);
		angleDeg += diff;

		(*stepAngleDegPtr) *= -1;
	}

	else if (angleDeg > maxServoAngleDeg) {
		uint16_t diff = abs(angleDeg - maxServoAngleDeg);
		angleDeg = min(maxServoAngleDeg, angleDeg);
		angleDeg -= diff;

		(*stepAngleDegPtr) *= -1;
	}


	(*anglePtr) = angleDeg;
	return 0;
}

void GetTrackRangeDeg(int16_t angleDeg, int16_t* minAngleDeg, int16_t* maxAngleDeg) {

	// set +- threshold to the angle
	uint16_t threshold = 10;

	*minAngleDeg = angleDeg - threshold;
	*maxAngleDeg = angleDeg + threshold;
}
