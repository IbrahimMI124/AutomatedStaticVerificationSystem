#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "plane_types.h"
#include "plane.h"

static bool autopilot_cpy;
static uint64_t altitude_cpy;

static bool recover_0_guard(void) {
  return !((!(autopilot_cpy)) || ((altitude_cpy) > ((uint64_t)(10000))));
}

static uint64_t recover_0_arg0(void) {
  return altitude_cpy;
}

static uint64_t recover_0_arg1(void) {
  return (uint64_t)(10000);
}

void step(void) {
  uint64_t recover_0_arg_temp0;
  uint64_t recover_0_arg_temp1;
  (autopilot_cpy) = (autopilot);
  (altitude_cpy) = (altitude);
  if ((recover_0_guard)()) {
    {(recover_0_arg_temp0) = ((recover_0_arg0)());
     (recover_0_arg_temp1) = ((recover_0_arg1)());
     (recover)((recover_0_arg_temp0), (recover_0_arg_temp1));}
  };
}
