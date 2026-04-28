/*
 * data.h
 *
 *  Created on: Apr 23, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_RADAR_EMULATOR_DATA_H_
#define INC_RADAR_EMULATOR_DATA_H_

#include <stdint.h>

typedef enum {
    ROTATION_CW = -1,
    ROTATION_CCW = 1
} RotationDir_t;

typedef struct {
    int16_t angleDeg;
    uint16_t distanceMm;
    RotationDir_t direction;
} TargetData;

#endif /* INC_RADAR_EMULATOR_DATA_H_ */
