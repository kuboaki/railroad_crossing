#include "app.h"

#include "stdbool.h"
#include "timer.h"
#include "horn.h"
#include "util.h"
#include "manual_switch.h"
#include "train_detector.h"
#include "train_checker.h"
#include "gate_rotator.h"


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
