/*
 * angle.h
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_ANGLE_H_
#define INC_ANGLE_H_

#include <stdint.h>

/**
 * @brief This function calculates each step angle to reach the range.
 * @param angleL1Deg first angle of range
 * @param angleL2Deg second angle of range
 * @param numTicks 	 number of step to meet end-to-end
 * @return 0:invalid data, != 0: step angle in degree
 */
int16_t CalculateStepAngle(
		int16_t angleL1Deg,
		int16_t angleL2Deg,
		uint32_t numTicks);
/**
 * @brief This function sets angle that is within the range and valid.
 * @param anglePtr 			angle to be set
 * @param stepAngleDegPtr 	each step angle to perform
 * @param minServoAngleDeg 	minimum angle of range
 * @param maxServoAngleDeg 	maximum angle of range
 * @return 0:success, != 0:failed
 */
uint16_t SetNextAngle(
		int16_t* anglePtr,
		int16_t* stepAngleDegPtr,
		int16_t minServoAngleDeg,
		int16_t maxServoAngleDeg);

void GetTrackRangeDeg(
		int16_t angleDeg,
		int16_t* minAngleDeg,
		int16_t* maxAngleDeg);

#endif /* INC_ANGLE_H_ */
