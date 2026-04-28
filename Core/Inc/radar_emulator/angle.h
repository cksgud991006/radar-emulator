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
 * The return rounds to the nearest integer and anticipates decimal points step error.
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
 * @param angleDegPtr 			angle to be set
 * @param stepAngleDegPtr 	each step angle to perform
 * @param minServoAngleDeg 	minimum angle of range
 * @param maxServoAngleDeg 	maximum angle of range
 * @return 0:success, != 0:failed
 */
uint16_t GetNextAngles(
		int16_t* angleDegPtr,
		int16_t* stepAngleDegPtr,
		int16_t minServoAngleDeg,
		int16_t maxServoAngleDeg);

/**
 * @brief This function sets angle that is within the range and valid.
 * @param angleDeg 			current angle
 * @param stepAngleDeg 		each step angle to perform
 * @param minServoAngleDeg 	minimum angle of range
 * @param maxServoAngleDeg 	maximum angle of range
 * @return nextAngleDeg		Next valid angle within the range

uint16_t GetNextAngle(
		int16_t angleDeg,
		int16_t stepAngleDeg,
		int16_t minServoAngleDeg,
		int16_t maxServoAngleDeg);
*/
/**
 * @brief This function returns starting angle given current angle and direction
 * @param anglePtr 			current angle
 * @param stepAngleDegPtr 	current angle direction
 * @return startAngleDeg	Angle to start from
 */
uint16_t GetStartAngleDeg(
		int16_t angleDeg,
		int16_t stepAngleDeg);

/**
 * @brief This function sets predetermined min/max range of given angle
 * @param angleDeg 				current angle
 * @param minAngleDegPtr 		minimum angle of range
 * @param maxAngleDegPtr 		maximum angle of range
 * @return
 */
void GetTrackRange(
		int16_t angleDeg,
		int16_t* minAngleDegPtr,
		int16_t* maxAngleDegPtr);

#endif /* INC_ANGLE_H_ */
