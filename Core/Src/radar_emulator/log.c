/*
 * log.c
 *
 *  Created on: Apr 24, 2026
 *      Author: Chanhyung Shim
 */

#include <radar_emulator/log.h>
#include <stdarg.h>

void Log_Format(LogMessage_t *msg, const char *fmt, ...) {
	msg->timestamp = xTaskGetTickCount();
	msg->level = 1;

	va_list args;
	va_start(args, fmt);
	vsnprintf(&(msg->message), sizeof(msg->message), fmt, args);
	va_end(args);
}
