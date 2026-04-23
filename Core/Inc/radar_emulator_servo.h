/*
 * radar_emulator_servo.h
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_RADAR_EMULATOR_SERVO_H_
#define INC_RADAR_EMULATOR_SERVO_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

// TIM_CLK = 84 MHz / 200 = 420 KHz = 2.38 microsec
// For 0.5 ms (0 degrees): 500 microsec / 2.38 microsec = 210 counts
// For 2.5 ms (180 degrees): 2500 microsec / 2.38 microsec = 1050 counts
void SetServoAngle(TIM_HandleTypeDef* htim, uint32_t channel, int16_t angleDeg);

#endif /* INC_RADAR_EMULATOR_SERVO_H_ */
