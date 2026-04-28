/*
 * sensor.h
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_RADAR_EMULATOR_SENSOR_H_
#define INC_RADAR_EMULATOR_SENSOR_H_

#include "stm32f4xx_hal.h"
#include <radar_emulator/data.h>
#include <stdbool.h>
#include <stdint.h>

void InitRadarEmulatorSensor(GPIO_TypeDef* port, uint16_t pin);

uint16_t GetMaxRange(
		uint16_t mode);

bool IsTargetValid(
		TargetData* targetData);


#endif /* INC_RADAR_EMULATOR_SENSOR_H_ */
