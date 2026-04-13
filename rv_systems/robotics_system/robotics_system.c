#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "robotics_system_types.h"
#include "robotics_system.h"

static float servo_cpy;
static float angle_cpy;
static bool e_stop_cpy;
static float motor_power_cpy;
static float s0[(1)] = {((float)(0.0f))};
static size_t s0_idx = (0);

static float s0_get(size_t x) {
  return (s0)[((s0_idx) + (x)) % ((size_t)(1))];
}

static float s0_gen(void) {
  return servo_cpy;
}

static bool viol_joint_limits_0_guard(void) {
  return !(((angle_cpy) >= ((float)(-1.5f))) && ((angle_cpy) <= ((float)(1.5f))));
}

static bool viol_servo_delta_1_guard(void) {
  return !(((((servo_cpy) - ((s0_get)((0)))) < ((float)(0.0f))) ? ((float)(0.0f)) - ((servo_cpy) - ((s0_get)((0)))) : ((servo_cpy) - ((s0_get)((0))))) <= ((float)(3.0f)));
}

static bool viol_estop_motor_power_2_guard(void) {
  return !((!(e_stop_cpy)) || ((motor_power_cpy) == ((float)(0.0f))));
}

void step(void) {
  float s0_tmp;
  (servo_cpy) = (servo);
  (angle_cpy) = (angle);
  (e_stop_cpy) = (e_stop);
  (motor_power_cpy) = (motor_power);
  if ((viol_joint_limits_0_guard)()) {
    {(viol_joint_limits)();}
  };
  if ((viol_servo_delta_1_guard)()) {
    {(viol_servo_delta)();}
  };
  if ((viol_estop_motor_power_2_guard)()) {
    {(viol_estop_motor_power)();}
  };
  (s0_tmp) = ((s0_gen)());
  ((s0)[s0_idx]) = (s0_tmp);
  (s0_idx) = (((s0_idx) + ((size_t)(1))) % ((size_t)(1)));
}
