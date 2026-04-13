#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "medical_system_types.h"
#include "medical_system.h"

static int32_t hr_cpy;
static bool pump_cpy;
static bool vitals_out_of_range_cpy;
static bool alarm_cpy;
static float rate_cpy;
static bool s1[(1)] = {(false)};
static uint32_t s0[(1)] = {((uint32_t)(0))};
static bool s2[(1)] = {(false)};
static bool s4[(1)] = {(false)};
static uint32_t s3[(1)] = {((uint32_t)(0))};
static bool s5[(1)] = {(false)};
static size_t s1_idx = (0);
static size_t s0_idx = (0);
static size_t s2_idx = (0);
static size_t s4_idx = (0);
static size_t s3_idx = (0);
static size_t s5_idx = (0);

static bool s1_get(size_t x) {
  return (s1)[((s1_idx) + (x)) % ((size_t)(1))];
}

static uint32_t s0_get(size_t x) {
  return (s0)[((s0_idx) + (x)) % ((size_t)(1))];
}

static bool s2_get(size_t x) {
  return (s2)[((s2_idx) + (x)) % ((size_t)(1))];
}

static bool s4_get(size_t x) {
  return (s4)[((s4_idx) + (x)) % ((size_t)(1))];
}

static uint32_t s3_get(size_t x) {
  return (s3)[((s3_idx) + (x)) % ((size_t)(1))];
}

static bool s5_get(size_t x) {
  return (s5)[((s5_idx) + (x)) % ((size_t)(1))];
}

static bool s1_gen(void) {
  return (hr_cpy) < ((int32_t)(40));
}

static uint32_t s0_gen(void) {
  return (((hr_cpy) < ((int32_t)(40))) && (!((s1_get)((0))))) ? (uint32_t)(10) : ((!(pump_cpy)) ? (uint32_t)(0) : ((((s0_get)((0))) > ((uint32_t)(0))) ? ((s0_get)((0))) - ((uint32_t)(1)) : ((uint32_t)(0))));
}

static bool s2_gen(void) {
  return (((hr_cpy) < ((int32_t)(40))) && (!((s1_get)((0))))) ? true : ((!(pump_cpy)) ? false : ((s2_get)((0))));
}

static bool s4_gen(void) {
  return vitals_out_of_range_cpy;
}

static uint32_t s3_gen(void) {
  return ((vitals_out_of_range_cpy) && (!((s4_get)((0))))) ? (uint32_t)(1) : ((alarm_cpy) ? (uint32_t)(0) : ((((s3_get)((0))) > ((uint32_t)(0))) ? ((s3_get)((0))) - ((uint32_t)(1)) : ((uint32_t)(0))));
}

static bool s5_gen(void) {
  return ((vitals_out_of_range_cpy) && (!((s4_get)((0))))) ? true : ((alarm_cpy) ? false : ((s5_get)((0))));
}

static bool viol_hr_pump_stop_0_guard(void) {
  return (((s0_get)((0))) == ((uint32_t)(0))) && (((s2_get)((0))) && (!(!(pump_cpy))));
}

static bool viol_infusion_rate_max_1_guard(void) {
  return !((rate_cpy) <= ((float)(5.0f)));
}

static bool viol_vitals_alarm_2_guard(void) {
  return (((s3_get)((0))) == ((uint32_t)(0))) && (((s5_get)((0))) && (!(alarm_cpy)));
}

void step(void) {
  bool s1_tmp;
  uint32_t s0_tmp;
  bool s2_tmp;
  bool s4_tmp;
  uint32_t s3_tmp;
  bool s5_tmp;
  (hr_cpy) = (hr);
  (pump_cpy) = (pump);
  (vitals_out_of_range_cpy) = (vitals_out_of_range);
  (alarm_cpy) = (alarm);
  (rate_cpy) = (rate);
  if ((viol_hr_pump_stop_0_guard)()) {
    {(viol_hr_pump_stop)();}
  };
  if ((viol_infusion_rate_max_1_guard)()) {
    {(viol_infusion_rate_max)();}
  };
  if ((viol_vitals_alarm_2_guard)()) {
    {(viol_vitals_alarm)();}
  };
  (s1_tmp) = ((s1_gen)());
  (s0_tmp) = ((s0_gen)());
  (s2_tmp) = ((s2_gen)());
  (s4_tmp) = ((s4_gen)());
  (s3_tmp) = ((s3_gen)());
  (s5_tmp) = ((s5_gen)());
  ((s1)[s1_idx]) = (s1_tmp);
  ((s0)[s0_idx]) = (s0_tmp);
  ((s2)[s2_idx]) = (s2_tmp);
  ((s4)[s4_idx]) = (s4_tmp);
  ((s3)[s3_idx]) = (s3_tmp);
  ((s5)[s5_idx]) = (s5_tmp);
  (s1_idx) = (((s1_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s0_idx) = (((s0_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s2_idx) = (((s2_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s4_idx) = (((s4_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s3_idx) = (((s3_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s5_idx) = (((s5_idx) + ((size_t)(1))) % ((size_t)(1)));
}
