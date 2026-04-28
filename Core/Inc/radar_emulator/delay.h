/*
 * delay.h
 *
 *  Created on: Apr 27, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_RADAR_EMULATOR_DELAY_H_
#define INC_RADAR_EMULATOR_DELAY_H_

#include <stdint.h>

uint32_t CalculateServoDelay(int16_t stepAngleDeg);
uint32_t CalculateTaskDelay(int16_t angleDeg, uint16_t steps);

#endif /* INC_RADAR_EMULATOR_DELAY_H_ */
