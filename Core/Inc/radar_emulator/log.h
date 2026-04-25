/*
 * log.h
 *
 *  Created on: Apr 24, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_RADAR_EMULATOR_LOG_H_
#define INC_RADAR_EMULATOR_LOG_H_

#include <stdint.h>

typedef struct {
    uint32_t timestamp;
    char message[64]; // Fixed size is safer for Queues
    uint8_t level;
} LogMessage_t;

void Log_Format(LogMessage_t *msg, const char *fmt, ...);

#endif /* INC_RADAR_EMULATOR_LOG_H_ */
