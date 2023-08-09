#ifndef TRAIN_CHECKER_H_
#define TRAIN_CHECKER_H_

#include "ev3api.h"
#include "stdbool.h"
#include "util.h"

extern void train_checker_init(void);
extern void train_checker_set_threshold(int threshold);
extern bool train_checker_is_detected(void);

#endif // TRAIN_CHECKER_H_
