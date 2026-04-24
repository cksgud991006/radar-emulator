/*
 * sensor.h
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_RADAR_EMULATOR_SENSOR_H_
#define INC_RADAR_EMULATOR_SENSOR_H_

#include <radar_emulator_data.h>
#include <stdbool.h>
#include <stdint.h>

void InitRadarEmulatorSensor();

uint16_t GetMaxRange(
		uint16_t mode);

bool IsTargetValid(
		TargetData* targetData);


#endif /* INC_RADAR_EMULATOR_SENSOR_H_ */
