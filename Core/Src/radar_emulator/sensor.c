/*
 * sensor.c
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#include "radar_emulator/sensor.h"
#include "VL53L1X_api.h"
#include "radar_emulator/config.h"

void InitRadarEmulatorSensor(GPIO_TypeDef* port, uint16_t pin) {

	VL53L1X_ERROR sensorStatus;

	// hardware reset (Replace GPIO_PORT/PIN with your actual connections)
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
	HAL_Delay(10);


	sensorStatus = VL53L1X_SensorInit(VL53L1X_ADDRESS);
	if (sensorStatus != 0) {
		return;
	}

	uint8_t sensorState = 0;
	while (sensorState == 0) {
		sensorStatus = VL53L1X_BootState(VL53L1X_ADDRESS, &sensorState);
		HAL_Delay(2); // 1.2ms ~ 1.5ms to reach booted state
	}

	sensorStatus = VL53L1X_ClearInterrupt(VL53L1X_ADDRESS);
	sensorStatus = VL53L1X_SetDistanceMode(VL53L1X_ADDRESS, SENSOR_MODE_DEFAULT);
	sensorStatus = VL53L1X_SetTimingBudgetInMs(VL53L1X_ADDRESS, SENSOR_SCAN_DELAY_MS);
	sensorStatus = VL53L1X_StartRanging(VL53L1X_ADDRESS);
}

uint16_t GetMaxRange(uint16_t mode) {

	uint16_t distanceMm = 0;

	switch(mode) {
		// short distance mode
		case 1:
			distanceMm = 1300; // ~1.3m
			break;
		// long distance mode
		case 2:
			distanceMm = 100; // ~4.0m
			break;

		default: distanceMm = 1300; // ~1.3m
	}

	return distanceMm;
}

bool IsTargetValid(TargetData* targetData) {

	// for short mode, ranging distance ensured is 40 mm ~ 1300 mm.
	// for long mode, ranging distance ensured is 40 mm ~ 4000 mm.
	VL53L1X_ERROR status;

	uint16_t distanceMm = targetData->distanceMm;
	// 1. timeout / no target
	if (distanceMm == 0xffff) return false;

	uint16_t mode;
	status = VL53L1X_GetDistanceMode(VL53L1X_ADDRESS, &mode);

	uint16_t minRangeMm = 0;
	uint16_t maxRangeMm = 0;

	maxRangeMm = GetMaxRange(mode);

	// 2. mode-based validation
	if (minRangeMm < distanceMm && distanceMm < maxRangeMm) return true;

	return false;

}
