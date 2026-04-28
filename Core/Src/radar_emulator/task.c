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
#include <radar_emulator/delay.h>
#include <radar_emulator/math.h>
#include <semphr.h>

#include "VL53L1X_api.h"
#include "radar_emulator/angle.h"

QueueHandle_t xTaskQueue;
QueueHandle_t xLogQueue;
SemaphoreHandle_t xSearchPermit;
SemaphoreHandle_t xTrackPermit;

void InitTask() {
	xTaskQueue = xQueueCreate(TASK_BUFFER_SIZE, sizeof(TargetData));
	xLogQueue = xQueueCreate(LOG_BUFFER_SIZE, sizeof(LogMessage_t));
	xSearchPermit = xSemaphoreCreateBinary();
	xTrackPermit = xSemaphoreCreateBinary();

	if (xSearchPermit != NULL && xTrackPermit != NULL) {
		xSemaphoreGive(xSearchPermit);
	}
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
	int16_t angleDeg = MIN_SEARCH_ANGLE_DEG;

	int16_t stepAngleDeg = CalculateStepAngle(MIN_SEARCH_ANGLE_DEG, MAX_SEARCH_ANGLE_DEG, GENERAL_SEARCH_STEPS);

	//uint32_t delay = max(CalculateServoDelay(stepAngleDeg), CalculateTaskDelay(angleDeg, TRACK_SEARCH_STEPS));

	//delay = max(delay, SENSOR_SCAN_DELAY_MS);

	uint32_t delay = max(CalculateServoDelay(stepAngleDeg), SENSOR_SCAN_DELAY_MS);

	VL53L1X_ERROR status;

	uint8_t isDataReady = 1;

	uint16_t distanceMm = 0;

	for(;;) {

		xSemaphoreTake(xSearchPermit, portMAX_DELAY);

		Log_Format(&msg, "Radar Search: Angle=%d, Dist=%d", angleDeg, distanceMm);

		xQueueSend(xLogQueue, &msg, 0);

		SetServoAngle(htim, channel, angleDeg);

		status = VL53L1X_CheckForDataReady(VL53L1X_ADDRESS, &isDataReady);

		if (isDataReady) {

			status = VL53L1X_GetDistance(VL53L1X_ADDRESS, &distanceMm);

			status = VL53L1X_ClearInterrupt(VL53L1X_ADDRESS);

			TargetData targetData = {
					.angleDeg = angleDeg,
					.distanceMm = distanceMm,
					.direction = stepAngleDeg > 0? ROTATION_CCW : ROTATION_CW
			};

			GetNextAngles(&angleDeg, &stepAngleDeg, MIN_SEARCH_ANGLE_DEG, MAX_SEARCH_ANGLE_DEG);

			if (ProcessAction(&targetData)) {
				status = xSemaphoreGive(xTrackPermit);
			}

			else {
				status = xSemaphoreGive(xSearchPermit);
			}
		}

		else {
			status = xSemaphoreGive(xSearchPermit);
		}

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

		status = xSemaphoreTake(xTrackPermit, portMAX_DELAY);

		status = xQueueReceive(xTaskQueue, &targetData, portMAX_DELAY);

		int16_t angleDeg = targetData.angleDeg;

		uint16_t distanceMm = targetData.distanceMm;

		int8_t direction = targetData.direction;

		int16_t minRangeDeg, maxRangeDeg;

		GetTrackRange(angleDeg, &minRangeDeg, &maxRangeDeg);

		int16_t stepAngleDeg = CalculateStepAngle(minRangeDeg, maxRangeDeg, TRACK_SEARCH_STEPS) * direction;

		uint32_t delay = CalculateServoDelay(stepAngleDeg);

		delay = max(delay, SENSOR_SCAN_DELAY_MS);

		angleDeg = GetStartAngleDeg(angleDeg, stepAngleDeg);

		for (int revisit=0; revisit < TRACK_SEARCH_REVISITS; ++revisit) {

			for (int s=0; s<TRACK_SEARCH_STEPS; ++s) {
				SetServoAngle(htim, channel, angleDeg);

				Log_Format(&msg, "Target Found: Angle=%d, Dist=%d", angleDeg, distanceMm);

				xQueueSend(xLogQueue, &msg, 0);

				status = VL53L1X_CheckForDataReady(VL53L1X_ADDRESS, &isDataReady);

				if (isDataReady) {

					sensorStatus = VL53L1X_GetDistance(VL53L1X_ADDRESS, &distanceMm);

					status = VL53L1X_ClearInterrupt(VL53L1X_ADDRESS);

					GetNextAngles(&angleDeg, &stepAngleDeg, minRangeDeg, maxRangeDeg);
				}
				// The task blocks every servo transit delay
				vTaskDelay(delay);
			}
		}

		xSemaphoreGive(xSearchPermit);
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
