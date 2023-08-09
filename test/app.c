#include "app.h"

#include "stdbool.h"
#include "timer.h"
#include "horn.h"
#include "util.h"

static const sensor_port_t manual_switch_sensor_port = EV3_PORT_1;
static const sensor_type_t manual_switch_sensor_type = TOUCH_SENSOR;

void manual_switch_init(void) {
    ev3_sensor_config(manual_switch_sensor_port, manual_switch_sensor_type);
}

bool manual_switch_is_pushed(void) {
    return ev3_touch_sensor_is_pressed(manual_switch_sensor_port);
}

#define CHECKER_THRESHOLD 0
static const sensor_port_t checker_sensor_A = EV3_PORT_2;
static const sensor_port_t checker_sensor_B = EV3_PORT_3;
static const sensor_type_t checker_sensor_type = COLOR_SENSOR;

void checker_init(void) {
    ev3_sensor_config(checker_sensor_A, checker_sensor_type);
    ev3_sensor_config(checker_sensor_B, checker_sensor_type);
}

bool checker_is_train_detected(sensor_port_t port) {
    // 通過していないときは0、通過中は1,2,...8などとな
    return ev3_color_sensor_get_reflect(port) > CHECKER_THRESHOLD;
}

#define DETECTOR_THRESHOLD 10
static const sensor_port_t detector_sensor_C = EV3_PORT_4;
static const sensor_type_t detector_sensor_type = ULTRASONIC_SENSOR;

void detector_init(void) {
    ev3_sensor_config(detector_sensor_C, detector_sensor_type);
    dly_tsk(500U * 1000U);
}

bool detector_is_train_detected(void) {
    // 通過していないときは13, 14 など、通過中は4,5,,,8 などとなる
    return ev3_ultrasonic_sensor_get_distance(detector_sensor_C) < DETECTOR_THRESHOLD;

    // 背が低い車両や、車両間の隙間の対策（一定時間の経過を判定する等）が必要そう
}

#define ROTATOR_DEGREE 90
int rotator_degree = ROTATOR_DEGREE;
#define ROTATOR_SPEED 8
int rotator_speed = ROTATOR_SPEED;

static const motor_port_t motor_port1 = EV3_PORT_A;
static const motor_port_t motor_port2 = EV3_PORT_B;
static const motor_type_t motor_type = LARGE_MOTOR;

void rotator_init(void) {
    ev3_motor_config(motor_port1, motor_type);
    ev3_motor_config(motor_port2, motor_type);
}

void rotator_rotate_normal(void) {
    // ev3_motor_set_power(motor_port1, ROTATOR_POWER);
    // ev3_motor_set_power(motor_port2, ROTATOR_POWER);
    ev3_motor_rotate(motor_port1, rotator_degree, rotator_speed, false);
    ev3_motor_rotate(motor_port2, rotator_degree, rotator_speed, false);
}

void rotator_rotate_reverse(void) {
    // ev3_motor_set_power(motor_port1,  -ROTATOR_POWER);
    // ev3_motor_set_power(motor_port2,  -ROTATOR_POWER);
    ev3_motor_rotate(motor_port1, -rotator_degree, rotator_speed, false);
    ev3_motor_rotate(motor_port2, -rotator_degree, rotator_speed, false);
}

void rotator_stop(void) {
    ev3_motor_stop(motor_port1, true);
    ev3_motor_stop(motor_port2, true);
}

typedef enum _test_state {
    TST_INIT,
    TST_OPEN,
    TST_OPEN2,
    TST_TO_OPEN,
    TST_CLOSE,
    TST_CLOSE2,
    TST_CLOSE3,
    TST_TO_CLOSE,
    TNUM_TST_STATE
} test_state;

static char* t_state_msg[TNUM_TST_STATE] = {
    "TST_INIT",
    "TST_OPEN",  "TST_OPEN2", "TST_TO_OPEN",
    "TST_CLOSE", "TST_CLOSE2",
    "TST_CLOSE3" "TST_TO_CLOSE"
};


static test_state t_state = TST_INIT;

void test_init(void) {
    rotator_init();
    manual_switch_init();
//    checker_init();
//    detector_init();
}

static bool t_is_entry = true;

void main_task(intptr_t unused) {
    if(t_is_entry) {
        msg_f(t_state_msg[t_state], 2);
    }

    switch(t_state) {
    case TST_INIT:
        if(t_is_entry) {
            t_is_entry = false;
            init_f("crossing test");
            msg_f(t_state_msg[t_state], 2);
            ev3_led_set_color(LED_ORANGE);
            test_init();
            timer_start(3000U*1000U);
        }
        if(timer_is_timedout()) {
            t_is_entry = true;
            timer_stop();
            t_state = TST_OPEN;
        }
        break;
    case TST_OPEN:
        if(t_is_entry) {
            t_is_entry = false;
            ev3_led_set_color(LED_GREEN);
            // horn_confirmation();
        }
        if(manual_switch_is_pushed()) {
            t_is_entry = true;
            t_state = TST_OPEN2;
        }
        /* if(checker_is_train_detected(checker_sensor_A) */
        /*   || checker_is_train_detected(checker_sensor_B)) { */
        /*     t_is_entry = true; */
        /*     t_state = TST_TO_CLOSE; */
        /* } */
        break;
    case TST_OPEN2:
        if(t_is_entry) {
            t_is_entry = false;
            ev3_led_set_color(LED_ORANGE);
        }
        if(!manual_switch_is_pushed()) {
            t_is_entry = true;
            t_state = TST_TO_CLOSE;
        }
        break;
    case TST_TO_CLOSE:
        if(t_is_entry) {
            t_is_entry = false;
            ev3_led_set_color(LED_ORANGE);
            // rotator_rotate_normal();
            // horn_warning();
            timer_start(3000U*1000U);
        }
        if(timer_is_timedout()) {
            t_is_entry = true;
            timer_stop();
            t_state = TST_CLOSE;
        }
        break;
    case TST_CLOSE:
        if(t_is_entry) {
            t_is_entry = false;
            ev3_led_set_color(LED_RED);
        }
        if(manual_switch_is_pushed()) {
            t_is_entry = true;
            t_state = TST_CLOSE3;
        }
        /* if(detector_is_train_detected()) { */
        /*     t_is_entry = true; */
        /*     t_state = TST_CLOSE2; */
        /* } */
        break;
    case TST_CLOSE3:
        if(t_is_entry) {
            t_is_entry = false;
            ev3_led_set_color(LED_ORANGE);
        }
        if(!manual_switch_is_pushed()){
            t_is_entry = true;
            t_state = TST_TO_OPEN;
        }
    case TST_CLOSE2:
        if(t_is_entry) {
            t_is_entry = false;
            ev3_led_set_color(LED_ORANGE);
        }
        /* if(!detector_is_train_detected()) { */
        /*     t_is_entry = true; */
        /*     t_state = TST_TO_OPEN; */
        /* } */
        break;
    case TST_TO_OPEN:
        if(t_is_entry) {
            t_is_entry = false;
            ev3_led_set_color(LED_ORANGE);
            // rotator_rotate_reverse();
            // horn_warning();
            timer_start(3000U*1000U);
        }
        if(timer_is_timedout()) {
            t_is_entry = true;
            timer_stop();
            t_state = TST_OPEN;
        }
        break;
    case TNUM_TST_STATE:
    default:
        break;
    }

    ext_tsk();
}
