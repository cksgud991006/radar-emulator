/*
 * math.h
 *
 *  Created on: Apr 24, 2026
 *      Author: Chanhyung Shim
 */

#ifndef INC_RADAR_EMULATOR_MATH_H_
#define INC_RADAR_EMULATOR_MATH_H_

// Source - https://stackoverflow.com/a/58532788
// Posted by Gabriel Staples, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-24, License - CC BY-SA 4.0

#define max(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})


#endif /* INC_RADAR_EMULATOR_MATH_H_ */
