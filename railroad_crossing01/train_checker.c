//
// train_checker.c
//

#include "train_checker.h"

static const sensor_port_t train_checker_sensor_A = EV3_PORT_3;
static const sensor_port_t train_checker_sensor_B = EV3_PORT_2;
static const sensor_type_t train_checker_sensor_type = COLOR_SENSOR;

#define TC_THRESHOLD 0
static int train_checker_threshold = TC_THRESHOLD;

void train_checker_set_threshold(int threshold) {
  train_checker_threshold = threshold;
}

void train_checker_set_to_current_A(void) {
  train_checker_threshold
    = ev3_color_sensor_get_reflect(train_checker_sensor_A);
}

void train_checker_set_to_current_B(void) {
  train_checker_threshold
    = ev3_color_sensor_get_reflect(train_checker_sensor_B);
}

static int8_t chk_old = 255;

void train_checker_init(void) {
    ev3_sensor_config(train_checker_sensor_A, train_checker_sensor_type);
    ev3_sensor_config(train_checker_sensor_B, train_checker_sensor_type);
}

bool train_checker_is_detected(sensor_port_t port) {
    // 通過していないときの値を設定し、通過中はそれより大きい値か調べる
    int8_t chk = ev3_color_sensor_get_reflect(port);
    int8_t line = 3;
    if(port == train_checker_sensor_B) { line = 4; }
    if( chk_old != chk ) {
        fmt_f("chk = %d", chk, line);
    }
    chk_old = chk;
    return chk > train_checker_threshold;
}

bool train_checker_is_detected_A(void) {
    return train_checker_is_detected(train_checker_sensor_A);
}
bool train_checker_is_detected_B(void) {
    return train_checker_is_detected(train_checker_sensor_B);
}
