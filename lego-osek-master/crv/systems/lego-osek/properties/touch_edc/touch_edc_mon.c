#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "touch_edc_mon_types.h"
#include "touch_edc_mon.h"

static bool fr_touch_cpy;
static int8_t fr_edc_flag_cpy;
static bool s0[(1)] = {(true)};
static bool s1[(1)] = {(false)};
static int8_t s2[(1)] = {((int8_t)(0))};
static size_t s0_idx = (0);
static size_t s1_idx = (0);
static size_t s2_idx = (0);

static bool s0_get(size_t x) {
  return (s0)[((s0_idx) + (x)) % ((size_t)(1))];
}

static bool s1_get(size_t x) {
  return (s1)[((s1_idx) + (x)) % ((size_t)(1))];
}

static int8_t s2_get(size_t x) {
  return (s2)[((s2_idx) + (x)) % ((size_t)(1))];
}

static bool s0_gen(void) {
  return false;
}

static bool s1_gen(void) {
  return fr_touch_cpy;
}

static int8_t s2_gen(void) {
  return fr_edc_flag_cpy;
}

static bool touch_edc_violation_0_guard(void) {
  return !(((s0_get)((0))) || (((!((fr_touch_cpy) && (!((s1_get)((0)))))) || ((fr_edc_flag_cpy) != ((s2_get)((0))))) && ((!((fr_touch_cpy) == ((s1_get)((0))))) || (!((fr_edc_flag_cpy) != ((s2_get)((0))))))));
}

void step(void) {
  bool s0_tmp;
  bool s1_tmp;
  int8_t s2_tmp;
  (fr_touch_cpy) = (fr_touch);
  (fr_edc_flag_cpy) = (fr_edc_flag);
  if ((touch_edc_violation_0_guard)()) {
    {(touch_edc_violation)();}
  };
  (s0_tmp) = ((s0_gen)());
  (s1_tmp) = ((s1_gen)());
  (s2_tmp) = ((s2_gen)());
  ((s0)[s0_idx]) = (s0_tmp);
  ((s1)[s1_idx]) = (s1_tmp);
  ((s2)[s2_idx]) = (s2_tmp);
  (s0_idx) = (((s0_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s1_idx) = (((s1_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s2_idx) = (((s2_idx) + ((size_t)(1))) % ((size_t)(1)));
}
