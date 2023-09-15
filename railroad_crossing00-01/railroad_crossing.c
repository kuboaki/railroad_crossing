#include "app.h"

#include "railroad_crossing.h"

typedef enum _railroad_crossing_state {
    RC_INIT,
    RC_WAIT_FOR_OPEN_A, RC_WAIT_FOR_OPEN_B,
    RC_OPENED, RC_TO_OPEN,
    RC_CLOSED_A, RC_CLOSED_B,
    RC_TO_CLOSE_A, RC_TO_CLOSE_B, TNUM_RC_STATE
} railroad_crossing_state;

static char* rc_state_msg[TNUM_RC_STATE] = {
    "RC_INIT",
    "RC_WAIT_FOR_OPEN_A", "RC_WAIT_FOR_OPEN_B",
    "RC_OPENED", "RC_TO_OPEN",
    "RC_CLOSED_A", "RC_CLOSED_B",
    "RC_TO_CLOSE_A", "RC_TO_CLOSE_B"
};

static railroad_crossing_state rc_state = RC_INIT;
static bool rc_is_entry = true;

#define ENTRY if(rc_is_entry){rc_is_entry=false;
#define DO }{
#define EVTCHK(f,s) if((f)){rc_state=(s);rc_is_entry=true;}
#define EXIT }if(rc_is_entry){
#define END }

void railroad_crossing_init(void) {
    gate_rotator_init();
    manual_switch_init();
    train_checker_init();
    train_detector_init();
    // warning_light_init();
    rc_state = RC_INIT;
}

static const int rc_timer_id = 0;

void railroad_crossing_run(void) {
    if(rc_is_entry) { msg_f(rc_state_msg[rc_state], 1); }

    gate_rotator_run();

    switch(rc_state) {
    case RC_INIT:
        ENTRY
            ev3_led_set_color(LED_ORANGE);
            railroad_crossing_init();
            timer_start(rc_timer_id, 3000U*1000U);
        DO
        EVTCHK(timer_is_timedout(rc_timer_id),RC_OPENED)
        EXIT
            timer_stop(rc_timer_id);
            train_checker_caribrate();
        END
        break;
    case RC_OPENED:
        ENTRY
            ev3_led_set_color(LED_GREEN);
            horn_confirmation();
        DO
        EVTCHK(train_checker_is_detected_A(),RC_TO_CLOSE_A)
        EVTCHK(train_checker_is_detected_B(),RC_TO_CLOSE_B)
        EXIT
        END
        break;
    case RC_TO_CLOSE_A:
        ENTRY
            horn_at_A();
            ev3_led_set_color(LED_ORANGE);
            train_checker_mask_B();
            gate_rotator_go_closing();
            // warning_light_flashing();
        DO
        EVTCHK(gate_rotator_is_closed(),RC_CLOSED_A)
        EXIT
            // timer_stop(rc_timer_id);
        END
        break;
    case RC_TO_CLOSE_B:
        ENTRY
            horn_at_B();
            ev3_led_set_color(LED_ORANGE);
            train_checker_mask_A();
            gate_rotator_go_closing();
            // warning_light_flashing();
            // timer_start(rc_timer_id,500U*1000U);
        DO
        // EVTCHK(timer_is_timedout(rc_timer_id),RC_CLOSED)
        EVTCHK(gate_rotator_is_closed(),RC_CLOSED_B)
        EXIT
            // timer_stop(rc_timer_id);
        END
        break;
    case RC_CLOSED_A:
        ENTRY
            ev3_led_set_color(LED_RED);
            horn_warning();
        DO
        EVTCHK(train_checker_is_detected_B(),RC_WAIT_FOR_OPEN_A)
        EXIT
        END
        break;
    case RC_CLOSED_B:
        ENTRY
            ev3_led_set_color(LED_RED);
            horn_warning();
        DO
        EVTCHK(train_checker_is_detected_A(),RC_WAIT_FOR_OPEN_B)
        EXIT
        END
        break;
    case RC_WAIT_FOR_OPEN_A:
        ENTRY
            timer_start(rc_timer_id,2000U*1000U);
        DO
        EVTCHK(timer_is_timedout(rc_timer_id),RC_TO_OPEN)
        EXIT
            timer_stop(rc_timer_id);
        END
        break;
case RC_TO_OPEN:
        ENTRY
            ev3_led_set_color(LED_ORANGE);
            gate_rotator_go_opening();
            train_checker_unmask();
            // warning_light_off();
        DO
        EVTCHK(timer_is_timedout(rc_timer_id),RC_TO_OPEN)
        EXIT
            // timer_stop(rc_timer_id);
        END
        break;
    case TNUM_RC_STATE:
    default:
        break;
    }

    ext_tsk();
}
