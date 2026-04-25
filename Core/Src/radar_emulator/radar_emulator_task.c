/*
 * radar_emulator_task.c
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */
#include <FreeRTOS.h>
#include <queue.h>
#include <radar_emulator/config.h>
#include <radar_emulator/sensor.h>
#include <radar_emulator/servo.h>
#include <radar_emulator/task.h>
#include <radar_emulator/log.h>
#include <semphr.h>

#include "VL53L1X_api.h"
#include "radar_emulator/angle.h"

static const UBaseType_t taskBufferSize = 10;
static const UBaseType_t logBufferSize = 50;
static const uint32_t steps = 3;
static const int16_t minServoAngleDeg = 0;
static const int16_t maxServoAngleDeg = 180;

QueueHandle_t xTaskQueue;
QueueHandle_t xLogQueue;
SemaphoreHandle_t xMutex;

void InitTask() {
	xTaskQueue = xQueueCreate(taskBufferSize, sizeof(TargetData));
	xLogQueue = xQueueCreate(logBufferSize, sizeof(LogMessage_t));
	xMutex = xSemaphoreCreateMutex();
}

BaseType_t ProcessAction(TargetData* targetData) {

	//VL53L1X_ERROR status;
	BaseType_t status;

	if (IsTargetValid(targetData)) {
		status = xQueueSendToFront(xTaskQueue, (void*) targetData, portMAX_DELAY);
		return status;
	}

	return pdFAIL;
}

void SearchTask(void *pvParameters) {

	LogMessage_t msg;

	ServoConfig_t *config = (ServoConfig_t *)pvParameters;

	TIM_HandleTypeDef* htim = config->timerHandle;
	uint32_t channel = config->channel;

	// The task blocks every sensor delay (~140 ms)
	int16_t angleDeg = minServoAngleDeg;

	int16_t stepAngleDeg = CalculateStepAngle(minServoAngleDeg, maxServoAngleDeg, 30);

	uint32_t delay = CalculateServoDelay(stepAngleDeg);

	VL53L1X_ERROR status;

	uint8_t isDataReady = 1;

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

			Log_Format(&msg, "Radar Angle: %d degrees", angleDeg);

			xQueueSend(xLogQueue, &msg, 0);

			SetNextAngle(&angleDeg, &stepAngleDeg, minServoAngleDeg, maxServoAngleDeg);

		}

		xSemaphoreGive(xMutex);

		// The task blocks every servo transit delay
		vTaskDelay(delay);
	}
}

void TrackTask(void *pvParameters) {

	LogMessage_t msg;

	ServoConfig_t *config = (ServoConfig_t *)pvParameters;

	TIM_HandleTypeDef* htim = config->timerHandle;
	uint32_t channel = config->channel;


	uint32_t status;

	uint8_t isDataReady = 0;

	VL53L1X_ERROR sensorStatus;

	TargetData targetData;

	for(;;) {

		status = xQueueReceive(xTaskQueue, &targetData, portMAX_DELAY);

		int16_t angleDeg = targetData.angleDeg;

		uint16_t distanceMm = targetData.distanceMm;

		int16_t minRangeDeg, maxRangeDeg;

		GetTrackRangeDeg(angleDeg, &minRangeDeg, &maxRangeDeg);

		int16_t stepAngleDeg = CalculateStepAngle(minRangeDeg, maxRangeDeg, 3);

		uint32_t delay = CalculateServoDelay(stepAngleDeg);

		int s=0;

		while (s < steps) {

			SetServoAngle(htim, channel, angleDeg);

			status = VL53L1X_CheckForDataReady(VL53L1X_ADDRESS, &isDataReady);

			if (isDataReady) {

				sensorStatus = VL53L1X_GetDistance(VL53L1X_ADDRESS, &distanceMm);

				Log_Format(&msg, "Target Found: Angle=%d, Dist=%.2f", angleDeg, distanceMm);

				xQueueSend(xLogQueue, &msg, 0);

				SetNextAngle(&angleDeg, &stepAngleDeg, minRangeDeg, maxRangeDeg);

				++s;
			}
			// The task blocks every servo transit delay
			vTaskDelay(delay);
		}
	}
}

void LogTask(void) {

	LogMessage_t log;

	for(;;) {
		if (xQueueReceive(xLogQueue, &log, portMAX_DELAY) == pdPASS) {
			printf("[%lu] LVL%d: %s\r\n",
					log.timestamp,
					log.level,
				    log.message);
		}
	}
}

void DeadlineCallback(void) {

}
