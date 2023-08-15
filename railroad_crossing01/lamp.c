//
//  lamp.c
//

#include "lamp.h"

static const motor_port_t motor_port1 = EV3_PORT_A;
static const motor_port_t motor_port2 = EV3_PORT_B;
static const motor_type_t motor_type = LARGE_MOTOR;

#define ROTATOR_POWER 8
static int16_t rotator_power = ROTATOR_POWER;
static int16_t rotate_count = 0;

void gate_rotator_init(void) {
    ev3_motor_config(motor_port1, motor_type);
    ev3_motor_config(motor_port2, motor_type);
    ev3_motor_reset_counts(morotr_port1);
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
