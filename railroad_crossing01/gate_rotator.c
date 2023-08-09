//
//  gate_rotator.c
//

#include "gate_rotator.h"

#define ROTATOR_DEGREE 90
int rotator_degree = ROTATOR_DEGREE;
#define ROTATOR_SPEED 8
int rotator_speed = ROTATOR_SPEED;

static const motor_port_t motor_port1 = EV3_PORT_A;
static const motor_port_t motor_port2 = EV3_PORT_B;
static const motor_type_t motor_type = LARGE_MOTOR;

void gate_rotator_init(void) {
    ev3_motor_config(motor_port1, motor_type);
    ev3_motor_config(motor_port2, motor_type);
}

void gate_rotator_rotate_normal(void) {
    ev3_motor_rotate(motor_port1, rotator_degree, rotator_speed, false);
    ev3_motor_rotate(motor_port2, rotator_degree, rotator_speed, false);
}

void gate_rotator_rotate_reverse(void) {
    ev3_motor_rotate(motor_port1, -rotator_degree, rotator_speed, false);
    ev3_motor_rotate(motor_port2, -rotator_degree, rotator_speed, false);
}

void gate_rotator_stop(void) {
    ev3_motor_stop(motor_port1, true);
    ev3_motor_stop(motor_port2, true);
}
