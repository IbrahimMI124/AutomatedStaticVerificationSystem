#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "plane2_types.h"
#include "plane2.h"

static bool autopilot_cpy;
static uint64_t altitude_cpy;
static bool s1[(1)] = {(false)};
static uint8_t s0[(1)] = {((uint8_t)(0))};
static size_t s1_idx = (0);
static size_t s0_idx = (0);

static bool s1_get(size_t x) {
  return (s1)[((s1_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s0_get(size_t x) {
  return (s0)[((s0_idx) + (x)) % ((size_t)(1))];
}

static bool s1_gen(void) {
  return autopilot_cpy;
}

static uint8_t s0_gen(void) {
  return ((autopilot_cpy) && (!((s1_get)((0))))) ? (uint8_t)(3) : (((altitude_cpy) > ((uint64_t)(10000))) ? (uint8_t)(0) : ((((s0_get)((0))) > ((uint8_t)(0))) ? ((s0_get)((0))) - ((uint8_t)(1)) : ((uint8_t)(0))));
}

static bool recover_0_guard(void) {
  return (((s0_get)((0))) == ((uint8_t)(1))) && (!((altitude_cpy) > ((uint64_t)(10000))));
}

static uint64_t recover_0_arg0(void) {
  return altitude_cpy;
}

static uint64_t recover_0_arg1(void) {
  return (uint64_t)(10000);
}

void step(void) {
  bool s1_tmp;
  uint8_t s0_tmp;
  uint64_t recover_0_arg_temp0;
  uint64_t recover_0_arg_temp1;
  (autopilot_cpy) = (autopilot);
  (altitude_cpy) = (altitude);
  if ((recover_0_guard)()) {
    {(recover_0_arg_temp0) = ((recover_0_arg0)());
     (recover_0_arg_temp1) = ((recover_0_arg1)());
     (recover)((recover_0_arg_temp0), (recover_0_arg_temp1));}
  };
  (s1_tmp) = ((s1_gen)());
  (s0_tmp) = ((s0_gen)());
  ((s1)[s1_idx]) = (s1_tmp);
  ((s0)[s0_idx]) = (s0_tmp);
  (s1_idx) = (((s1_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s0_idx) = (((s0_idx) + ((size_t)(1))) % ((size_t)(1)));
}
