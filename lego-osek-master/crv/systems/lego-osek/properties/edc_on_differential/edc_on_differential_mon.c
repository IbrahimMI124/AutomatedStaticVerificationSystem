#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "edc_on_differential_mon_types.h"
#include "edc_on_differential_mon.h"

static int8_t fr_edc_flag_cpy;
static int32_t fr_steering_angle_cpy;
static int32_t fr_cmd_speed_cpy;
static int32_t fr_motor_left_cpy;
static int32_t fr_motor_right_cpy;

static bool edc_on_differential_violation_0_guard(void) {
  return !((!(((fr_edc_flag_cpy) == ((int8_t)(-1))) && (((fr_steering_angle_cpy) > ((int32_t)(2))) || ((fr_steering_angle_cpy) < ((int32_t)(-2)))))) || (((fr_cmd_speed_cpy) == ((int32_t)(0))) || ((fr_motor_left_cpy) != (fr_motor_right_cpy))));
}

void step(void) {
  (fr_edc_flag_cpy) = (fr_edc_flag);
  (fr_steering_angle_cpy) = (fr_steering_angle);
  (fr_cmd_speed_cpy) = (fr_cmd_speed);
  (fr_motor_left_cpy) = (fr_motor_left);
  (fr_motor_right_cpy) = (fr_motor_right);
  if ((edc_on_differential_violation_0_guard)()) {
    {(edc_on_differential_violation)();}
  };
}
