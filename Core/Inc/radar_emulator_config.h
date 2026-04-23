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

typedef struct {
    TIM_HandleTypeDef* timerHandle; // e.g., &htim2
    uint32_t channel;               // e.g., TIM_CHANNEL_1
} ServoConfig_t;

#endif /* INC_RADAR_EMULATOR_CONFIG_H_ */
