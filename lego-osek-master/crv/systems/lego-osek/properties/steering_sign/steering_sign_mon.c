#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "steering_sign_mon_types.h"
#include "steering_sign_mon.h"

static int32_t fr_cmd_steer_cpy;
static int32_t fr_steering_angle_cpy;
static int32_t fr_motor_steer_cpy;

static bool steering_sign_violation_0_guard(void) {
  return !(((((((fr_cmd_steer_cpy) * ((int32_t)(40))) / ((int32_t)(100))) - (fr_steering_angle_cpy)) <= ((int32_t)(0))) || ((fr_motor_steer_cpy) > ((int32_t)(0)))) && (((((((fr_cmd_steer_cpy) * ((int32_t)(40))) / ((int32_t)(100))) - (fr_steering_angle_cpy)) >= ((int32_t)(0))) || ((fr_motor_steer_cpy) < ((int32_t)(0)))) && ((((((fr_cmd_steer_cpy) * ((int32_t)(40))) / ((int32_t)(100))) - (fr_steering_angle_cpy)) != ((int32_t)(0))) || ((fr_motor_steer_cpy) == ((int32_t)(0))))));
}

void step(void) {
  (fr_cmd_steer_cpy) = (fr_cmd_steer);
  (fr_steering_angle_cpy) = (fr_steering_angle);
  (fr_motor_steer_cpy) = (fr_motor_steer);
  if ((steering_sign_violation_0_guard)()) {
    {(steering_sign_violation)();}
  };
}
