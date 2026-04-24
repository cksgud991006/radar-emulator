/*
 * radar_emulator_task.c
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>

#include "radar_emulator_task.h"
#include "VL53L1X_api.h"
#include "angle.h"
#include "radar_emulator_config.h"
#include "radar_emulator_sensor.h"
#include "radar_emulator_servo.h"

extern QueueHandle_t xQueue;
extern SemaphoreHandle_t xMutex;

static const uint32_t steps = 3;
static const int16_t minServoAngleDeg = 0;
static const int16_t maxServoAngleDeg = 180;

BaseType_t ProcessAction(TargetData* targetData) {

	//VL53L1X_ERROR status;
	BaseType_t status;

	if (IsTargetValid(targetData)) {
		status = xQueueSendToFront(xQueue, (void*) targetData, 0);
		return status;
	}

	return pdFAIL;
}

void SearchTask(void *pvParameters) {

	ServoConfig_t *config = (ServoConfig_t *)pvParameters;

	TIM_HandleTypeDef* htim = config->timerHandle;
	uint32_t channel = config->channel;

	// The task blocks every sensor delay (~140 ms)
	int16_t angleDeg = minServoAngleDeg;

	int16_t stepAngleDeg = CalculateStepAngle(minServoAngleDeg, maxServoAngleDeg, 30);

	VL53L1X_ERROR status;

	uint8_t isDataReady = 0;

	uint16_t distanceMm = 0;

	for(;;) {

		xSemaphoreTake(xMutex, portMAX_DELAY);

		SetServoAngle(htim, channel, angleDeg);

		status = VL53L1X_CheckForDataReady(VL53L1X_ADDRESS, &isDataReady);

		if (isDataReady) {

			status = VL53L1X_GetDistance(VL53L1X_ADDRESS, &distanceMm);

			TargetData targetData = {
					.angleDeg = angleDeg,
					.distanceMm = distanceMm
			};

			ProcessAction(&targetData);

			SetNextAngle(&angleDeg, &stepAngleDeg, minServoAngleDeg, maxServoAngleDeg);

		}

		xSemaphoreGive(xMutex);
	}

}

void TrackTask(void *pvParameters) {

	ServoConfig_t *config = (ServoConfig_t *)pvParameters;

	TIM_HandleTypeDef* htim = config->timerHandle;
	uint32_t channel = config->channel;


	uint32_t status;

	VL53L1X_ERROR sensorStatus;

	TargetData targetData;

	for(;;) {

		status = xQueueReceive(xQueue, &targetData, 0);

		int16_t angleDeg = targetData.angleDeg;

		uint16_t distanceMm = targetData.distanceMm;

		int16_t minRangeDeg, maxRangeDeg;

		GetTrackRangeDeg(angleDeg, &minRangeDeg, &maxRangeDeg);

		int16_t stepAngleDeg = CalculateStepAngle(minRangeDeg, maxRangeDeg, 3);

		int s=0;

		while (s < steps) {

			SetServoAngle(htim, channel, angleDeg);

			sensorStatus = VL53L1X_GetDistance(VL53L1X_ADDRESS, &distanceMm);

			SetNextAngle(&angleDeg, &stepAngleDeg, minRangeDeg, maxRangeDeg);

			++s;
		}
	}
}

void LogTask(void) {

}

void DeadlineCallback(void) {

}
