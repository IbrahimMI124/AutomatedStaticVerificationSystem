#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "avionics_system_types.h"
#include "avionics_system.h"

static float airspeed_cpy;
static bool control_cpy;
static float altitude_cpy;
static bool s1[(1)] = {(false)};
static uint32_t s0[(1)] = {((uint32_t)(0))};
static bool s2[(1)] = {(false)};
static size_t s1_idx = (0);
static size_t s0_idx = (0);
static size_t s2_idx = (0);

static bool s1_get(size_t x) {
  return (s1)[((s1_idx) + (x)) % ((size_t)(1))];
}

static uint32_t s0_get(size_t x) {
  return (s0)[((s0_idx) + (x)) % ((size_t)(1))];
}

static bool s2_get(size_t x) {
  return (s2)[((s2_idx) + (x)) % ((size_t)(1))];
}

static bool s1_gen(void) {
  return (airspeed_cpy) < ((float)(50.0f));
}

static uint32_t s0_gen(void) {
  return (((airspeed_cpy) < ((float)(50.0f))) && (!((s1_get)((0))))) ? (uint32_t)(2) : ((control_cpy) ? (uint32_t)(0) : ((((s0_get)((0))) > ((uint32_t)(0))) ? ((s0_get)((0))) - ((uint32_t)(1)) : ((uint32_t)(0))));
}

static bool s2_gen(void) {
  return (((airspeed_cpy) < ((float)(50.0f))) && (!((s1_get)((0))))) ? true : ((control_cpy) ? false : ((s2_get)((0))));
}

static bool viol_altitude_min_0_guard(void) {
  return !((altitude_cpy) >= ((float)(300.0f)));
}

static bool viol_stall_control_1_guard(void) {
  return (((s0_get)((0))) == ((uint32_t)(0))) && (((s2_get)((0))) && (!(control_cpy)));
}

void step(void) {
  bool s1_tmp;
  uint32_t s0_tmp;
  bool s2_tmp;
  (airspeed_cpy) = (airspeed);
  (control_cpy) = (control);
  (altitude_cpy) = (altitude);
  if ((viol_altitude_min_0_guard)()) {
    {(viol_altitude_min)();}
  };
  if ((viol_stall_control_1_guard)()) {
    {(viol_stall_control)();}
  };
  (s1_tmp) = ((s1_gen)());
  (s0_tmp) = ((s0_gen)());
  (s2_tmp) = ((s2_gen)());
  ((s1)[s1_idx]) = (s1_tmp);
  ((s0)[s0_idx]) = (s0_tmp);
  ((s2)[s2_idx]) = (s2_tmp);
  (s1_idx) = (((s1_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s0_idx) = (((s0_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s2_idx) = (((s2_idx) + ((size_t)(1))) % ((size_t)(1)));
}
