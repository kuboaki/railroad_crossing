#ifndef ROTATOR_H_
#define ROTATOR_H_
//
//  gate_rotator.h
//

#include "ev3api.h"
#include "stdbool.h"
#include "util.h"

extern void rotator_init(void);

extern void rotator_rotate_normal(void);
extern void rotator_rotate_reverse(void);
extern void rotator_stop(void);

#endif // ROTATOR_H_
