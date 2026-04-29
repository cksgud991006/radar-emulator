/*
 * radar_emulator_task.h
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_RADAR_EMULATOR_TASK_H_
#define INC_RADAR_EMULATOR_TASK_H_

#include <radar_emulator/data.h>
#include <stdint.h>

void InitTask();

void SearchTask(void *pvParameters);

void TrackTask(void *pvParameters);

void LogTask(void *pvParameters);

void DeadlineCallback(void);

#endif /* INC_RADAR_EMULATOR_TASK_H_ */
