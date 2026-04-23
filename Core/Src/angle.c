/*
 * angle.c
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#include "angle.h"

uint16_t CalculateStepAngle(int16_t angleL1Deg, int16_t angleL2Deg, uint32_t numTicks) {

	uint16_t angleDeg = abs(angleL1Deg - angleL2Deg);

	// TODO: find valid step angle
	return angleDeg /  numTicks;

}

void SetNextAngle(int16_t* anglePtr, uint16_t* stepAngleDeg, int16_t minServoAngleDeg, int16_t maxServoAngleDeg) {

	// switch angle direction when angle moves beyond the limit
	int16_t angleDeg = *anglePtr;

	angleDeg += stepAngleDeg;

	if (angleDeg < minServoAngleDeg || angleDeg > maxServoAngleDeg) (*stepAngleDeg) *= -1;

	(*anglePtr) += stepAngleDeg;
}

void GetTrackRangeDeg(int16_t angleDeg, int16_t* minAngleDeg, int16_t* maxAngleDeg) {

	// set +- threshold to the angle
	uint16_t threshold = 10;

	*minAngleDeg = angleDeg - threshold;
	*maxAngleDeg = angleDeg + threshold;
}
