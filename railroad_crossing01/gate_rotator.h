#ifndef ROTATOR_H_
#define ROTATOR_H_
//
//  gate_rotator.h
//

#include "ev3api.h"
#include "stdbool.h"
#include "util.h"

extern void gate_rotator_init(void);

extern void gate_rotator_rotate_normal(void);
extern void gate_rotator_rotate_reverse(void);
extern void gate_rotator_stop(void);

#endif // ROTATOR_H_
