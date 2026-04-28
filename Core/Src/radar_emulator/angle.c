/*
 * angle.c
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#include <stdlib.h>
#include <math.h>
#include "radar_emulator/config.h"
#include "radar_emulator/angle.h"
#include "radar_emulator/math.h"

int16_t CalculateStepAngle(int16_t angleL1Deg, int16_t angleL2Deg, uint32_t numTicks) {

	if (numTicks == 0) return 0;

	uint16_t angleDeg = abs(angleL1Deg - angleL2Deg);

	return angleDeg / numTicks;

}

uint16_t GetNextAngles(int16_t* angleDegPtr, int16_t* stepAngleDegPtr, int16_t minServoAngleDeg, int16_t maxServoAngleDeg) {
	// set the range limit and switch angle direction when angle moves beyond the limit

	if (abs(*stepAngleDegPtr) > abs(maxServoAngleDeg - minServoAngleDeg)) return 1;

	int16_t angleDeg = *angleDegPtr;

	angleDeg += *stepAngleDegPtr;

	minServoAngleDeg = max(MIN_SEARCH_ANGLE_DEG, minServoAngleDeg);
	maxServoAngleDeg = min(MAX_SEARCH_ANGLE_DEG, maxServoAngleDeg);

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


	(*angleDegPtr) = angleDeg;
	return 0;
}
/*
uint16_t GetNextAngle(int16_t angleDeg, int16_t stepAngleDeg, int16_t minServoAngleDeg, int16_t maxServoAngleDeg) {
	// set the range limit and switch angle direction when angle moves beyond the limit

	if (abs(stepAngleDeg) > abs(maxServoAngleDeg - minServoAngleDeg)) return 1;

	angleDeg += stepAngleDeg;

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


	(*angleDegPtr) = angleDeg;
	return 0;
}
*/
uint16_t GetStartAngleDeg(int16_t angleDeg, int16_t stepAngleDeg) {

	int16_t angleDirection = max(stepAngleDeg, 0);

	int16_t startAngleDeg = angleDeg - stepAngleDeg;

	// cw
	if (angleDirection > 0) {
		return max(startAngleDeg, 0);
	}

	// ccw
	if (angleDirection < 0) {
		return min(startAngleDeg, 180);
	}

	return 0;
}

void GetTrackRange(int16_t angleDeg, int16_t* minAngleDegPtr, int16_t* maxAngleDegPtr) {

	uint16_t threshold = abs(MAX_SEARCH_ANGLE_DEG - MIN_SEARCH_ANGLE_DEG) / GENERAL_SEARCH_STEPS;

	*minAngleDegPtr = angleDeg - threshold;
	*maxAngleDegPtr = angleDeg + threshold;
}
