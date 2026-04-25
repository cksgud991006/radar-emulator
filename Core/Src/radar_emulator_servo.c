/*
 * radar_emulator_servo.c
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#include "radar_emulator_servo.h"

void SetServoAngle(TIM_HandleTypeDef* htim, uint32_t channel, int16_t angleDeg) {
	// map angle (0-180) to PWM (210 - 1050 counts)
	// 210 for 0.5 ms (0 degrees) and 1050 for 2.5 ms (180 degrees)
	uint32_t pulseLength = 210 + (angleDeg * (1050 - 210)/180);

	__HAL_TIM_SET_COMPARE(htim, channel, pulseLength);
}

uint32_t CalculateServoDelay(int16_t angleDeg) {
	// servo operates transit speed of 2 ms per degree
	uint32_t ticksMs = abs(angleDeg * 2);

	return ticksMs;
}
