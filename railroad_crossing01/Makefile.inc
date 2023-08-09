mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS += util.o timer.o horn.o manual_switch.o train_detector.o train_checker.o gate_rotator.o

SRCLANG := c

# APPL_DIR += $(mkfile_path)app

# INCLUDES += \
# 	-I$(mkfile_path)app \
