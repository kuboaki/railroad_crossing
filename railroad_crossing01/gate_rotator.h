#ifndef ROTATOR_H_
#define ROTATOR_H_
//
//  gate_rotator.h
//

#include "ev3api.h"
#include "stdbool.h"
#include "util.h"

<<<<<<< HEAD
extern void gate_rotator_open_gate(void);
extern void gate_rotator_close_gate(void);
=======
extern void gate_rotator_go_opening(void);
extern void gate_rotator_go_closing(void);
extern bool gate_rotator_is_closed(void);
extern bool gate_rotator_is_opened(void);
>>>>>>> 642077355d32ad46698b4867d897b5e7b64361f1
extern void gate_rotator_init(void);
extern void gate_rotator_stop(void);
extern void gate_rotator_run(void);

#endif // ROTATOR_H_
