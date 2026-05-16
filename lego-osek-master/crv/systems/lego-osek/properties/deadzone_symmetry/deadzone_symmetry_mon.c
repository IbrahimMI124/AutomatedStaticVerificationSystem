#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "deadzone_symmetry_mon_types.h"
#include "deadzone_symmetry_mon.h"

static int32_t fr_steering_angle_cpy;
static int32_t fr_motor_left_cpy;
static int32_t fr_motor_right_cpy;

static bool deadzone_symmetry_violation_0_guard(void) {
  return !((!(((fr_steering_angle_cpy) <= ((int32_t)(2))) && ((fr_steering_angle_cpy) >= ((int32_t)(-2))))) || ((fr_motor_left_cpy) == (fr_motor_right_cpy)));
}

void step(void) {
  (fr_steering_angle_cpy) = (fr_steering_angle);
  (fr_motor_left_cpy) = (fr_motor_left);
  (fr_motor_right_cpy) = (fr_motor_right);
  if ((deadzone_symmetry_violation_0_guard)()) {
    {(deadzone_symmetry_violation)();}
  };
}
