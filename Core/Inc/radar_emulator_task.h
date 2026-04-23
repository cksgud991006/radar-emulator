/*
 * radar_emulator_task.h
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_RADAR_EMULATOR_TASK_H_
#define INC_RADAR_EMULATOR_TASK_H_

#include <stdint.h>
#include "radar_emulator_data.h"

static const uint32_t steps = 3;

static const int16_t minServoAngleDeg = 0;

static const int16_t maxServoAngleDeg = 180;

BaseType_t ProcessAction(TargetData* targetData);

void SearchTask(void *pvParameters);

void TrackTask(void *pvParameters);

void LogTask(void);

void DeadlineCallback(void);

#endif /* INC_RADAR_EMULATOR_TASK_H_ */
