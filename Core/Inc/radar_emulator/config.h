/*
 * config.h
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_RADAR_EMULATOR_CONFIG_H_
#define INC_RADAR_EMULATOR_CONFIG_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define TASK_BUFFER_SIZE				1
#define LOG_BUFFER_SIZE					50
#define GENERAL_SEARCH_STEPS			20
#define TRACK_SEARCH_STEPS				5
#define MIN_SEARCH_ANGLE_DEG			0
#define MAX_SEARCH_ANGLE_DEG			180
#define MOTION_SMOOTHING_FACTOR			1
#define SENSOR_SCAN_DELAY_MS			140

typedef struct {
    TIM_HandleTypeDef* timerHandle; // e.g., &htim2
    uint32_t channel;               // e.g., TIM_CHANNEL_1
} ServoConfig_t;

#endif /* INC_RADAR_EMULATOR_CONFIG_H_ */
