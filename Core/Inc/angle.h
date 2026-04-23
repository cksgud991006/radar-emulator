/*
 * angle.h
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_ANGLE_H_
#define INC_ANGLE_H_

#include <stdint.h>

uint16_t CalculateStepAngle(
		int16_t angleL1Deg,
		int16_t angleL2Deg,
		uint32_t numTicks);

void SetNextAngle(
		int16_t* anglePtr,
		uint16_t* stepAngleDeg,
		int16_t minServoAngleDeg,
		int16_t maxServoAngleDeg);

void GetTrackRangeDeg(
		int16_t angleDeg,
		int16_t* minAngleDeg,
		int16_t* maxAngleDeg);

#endif /* INC_ANGLE_H_ */
