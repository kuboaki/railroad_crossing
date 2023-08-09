//
// train_checker.c
//

#include "train_checker.h"

static const sensor_port_t train_checker_sensor_A = EV3_PORT_2;
static const sensor_port_t train_checker_sensor_B = EV3_PORT_3;
static const sensor_type_t train_checker_sensor_type = COLOR_SENSOR;

#define TC_THRESHOLD 0
static int train_checker_threshold = TC_THRESHOLD;

void train_checker_set_threshold(int threshold) {
  train_checker_threshold = threshold;
}

void checker_init(void) {
    ev3_sensor_config(train_checker_sensor_A, train_checker_sensor_type);
    ev3_sensor_config(train_checker_sensor_B, train_checker_sensor_type);
}

bool checker_is_train_detected(sensor_port_t port) {
    // 通過していないときは0、通過中は1,2,...8などとなる
    return ev3_color_sensor_get_reflect(port) > TC_THRESHOLD;
}
