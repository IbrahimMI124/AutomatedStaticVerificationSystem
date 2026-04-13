#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "control_system_types.h"
#include "control_system.h"

static bool startup_cpy;
static float output_cpy;
static float ref_cpy;
static float u_cpy;
static bool disturbance_cpy;
static float velocity_cpy;
static float control_cpy;
static uint32_t s0[(1)] = {((uint32_t)(0))};
static bool s2[(1)] = {(false)};
static uint32_t s1[(1)] = {((uint32_t)(0))};
static bool s3[(1)] = {(false)};
static float s4[(1)] = {((float)(0.0f))};
static bool s6[(1)] = {(false)};
static uint32_t s5[(1)] = {((uint32_t)(0))};
static bool s7[(1)] = {(false)};
static size_t s0_idx = (0);
static size_t s2_idx = (0);
static size_t s1_idx = (0);
static size_t s3_idx = (0);
static size_t s4_idx = (0);
static size_t s6_idx = (0);
static size_t s5_idx = (0);
static size_t s7_idx = (0);

static uint32_t s0_get(size_t x) {
  return (s0)[((s0_idx) + (x)) % ((size_t)(1))];
}

static bool s2_get(size_t x) {
  return (s2)[((s2_idx) + (x)) % ((size_t)(1))];
}

static uint32_t s1_get(size_t x) {
  return (s1)[((s1_idx) + (x)) % ((size_t)(1))];
}

static bool s3_get(size_t x) {
  return (s3)[((s3_idx) + (x)) % ((size_t)(1))];
}

static float s4_get(size_t x) {
  return (s4)[((s4_idx) + (x)) % ((size_t)(1))];
}

static bool s6_get(size_t x) {
  return (s6)[((s6_idx) + (x)) % ((size_t)(1))];
}

static uint32_t s5_get(size_t x) {
  return (s5)[((s5_idx) + (x)) % ((size_t)(1))];
}

static bool s7_get(size_t x) {
  return (s7)[((s7_idx) + (x)) % ((size_t)(1))];
}

static uint32_t s0_gen(void) {
  return ((s0_get)((0))) + ((uint32_t)(1));
}

static bool s2_gen(void) {
  return startup_cpy;
}

static uint32_t s1_gen(void) {
  return ((startup_cpy) && (!((s2_get)((0))))) ? (uint32_t)(50) : ((((((output_cpy) - (ref_cpy)) < ((float)(0.0f))) ? ((float)(0.0f)) - ((output_cpy) - (ref_cpy)) : ((output_cpy) - (ref_cpy))) <= (((float)(5.0e-2f)) * (ref_cpy))) ? (uint32_t)(0) : ((((s1_get)((0))) > ((uint32_t)(0))) ? ((s1_get)((0))) - ((uint32_t)(1)) : ((uint32_t)(0))));
}

static bool s3_gen(void) {
  return ((startup_cpy) && (!((s2_get)((0))))) ? true : ((((((output_cpy) - (ref_cpy)) < ((float)(0.0f))) ? ((float)(0.0f)) - ((output_cpy) - (ref_cpy)) : ((output_cpy) - (ref_cpy))) <= (((float)(5.0e-2f)) * (ref_cpy))) ? false : ((s3_get)((0))));
}

static float s4_gen(void) {
  return u_cpy;
}

static bool s6_gen(void) {
  return disturbance_cpy;
}

static uint32_t s5_gen(void) {
  return ((disturbance_cpy) && (!((s6_get)((0))))) ? (uint32_t)(5) : (((((velocity_cpy) < ((float)(0.0f))) ? ((float)(0.0f)) - (velocity_cpy) : (velocity_cpy)) <= ((float)(1.0f))) ? (uint32_t)(0) : ((((s5_get)((0))) > ((uint32_t)(0))) ? ((s5_get)((0))) - ((uint32_t)(1)) : ((uint32_t)(0))));
}

static bool s7_gen(void) {
  return ((disturbance_cpy) && (!((s6_get)((0))))) ? true : (((((velocity_cpy) < ((float)(0.0f))) ? ((float)(0.0f)) - (velocity_cpy) : (velocity_cpy)) <= ((float)(1.0f))) ? false : ((s7_get)((0))));
}

static bool viol_post_condition_0_guard(void) {
  return !((((s0_get)((0))) < ((uint32_t)(50))) || (((((output_cpy) - (ref_cpy)) < ((float)(0.0f))) ? ((float)(0.0f)) - ((output_cpy) - (ref_cpy)) : ((output_cpy) - (ref_cpy))) <= (((float)(5.0e-2f)) * (ref_cpy))));
}

static bool viol_startup_stabilize_1_guard(void) {
  return (((s1_get)((0))) == ((uint32_t)(0))) && (((s3_get)((0))) && (!(((((output_cpy) - (ref_cpy)) < ((float)(0.0f))) ? ((float)(0.0f)) - ((output_cpy) - (ref_cpy)) : ((output_cpy) - (ref_cpy))) <= (((float)(5.0e-2f)) * (ref_cpy)))));
}

static bool viol_actuator_saturation_2_guard(void) {
  return !(((control_cpy) >= ((float)(-100.0f))) && ((control_cpy) <= ((float)(100.0f))));
}

static bool viol_temporal_consistency_3_guard(void) {
  return !(((((u_cpy) - ((s4_get)((0)))) < ((float)(0.0f))) ? ((float)(0.0f)) - ((u_cpy) - ((s4_get)((0)))) : ((u_cpy) - ((s4_get)((0))))) <= ((float)(3.0f)));
}

static bool viol_bounded_stabilisation_4_guard(void) {
  return (((s5_get)((0))) == ((uint32_t)(0))) && (((s7_get)((0))) && (!((((velocity_cpy) < ((float)(0.0f))) ? ((float)(0.0f)) - (velocity_cpy) : (velocity_cpy)) <= ((float)(1.0f)))));
}

void step(void) {
  uint32_t s0_tmp;
  bool s2_tmp;
  uint32_t s1_tmp;
  bool s3_tmp;
  float s4_tmp;
  bool s6_tmp;
  uint32_t s5_tmp;
  bool s7_tmp;
  (startup_cpy) = (startup);
  (output_cpy) = (output);
  (ref_cpy) = (ref);
  (u_cpy) = (u);
  (disturbance_cpy) = (disturbance);
  (velocity_cpy) = (velocity);
  (control_cpy) = (control);
  if ((viol_post_condition_0_guard)()) {
    {(viol_post_condition)();}
  };
  if ((viol_startup_stabilize_1_guard)()) {
    {(viol_startup_stabilize)();}
  };
  if ((viol_actuator_saturation_2_guard)()) {
    {(viol_actuator_saturation)();}
  };
  if ((viol_temporal_consistency_3_guard)()) {
    {(viol_temporal_consistency)();}
  };
  if ((viol_bounded_stabilisation_4_guard)()) {
    {(viol_bounded_stabilisation)();}
  };
  (s0_tmp) = ((s0_gen)());
  (s2_tmp) = ((s2_gen)());
  (s1_tmp) = ((s1_gen)());
  (s3_tmp) = ((s3_gen)());
  (s4_tmp) = ((s4_gen)());
  (s6_tmp) = ((s6_gen)());
  (s5_tmp) = ((s5_gen)());
  (s7_tmp) = ((s7_gen)());
  ((s0)[s0_idx]) = (s0_tmp);
  ((s2)[s2_idx]) = (s2_tmp);
  ((s1)[s1_idx]) = (s1_tmp);
  ((s3)[s3_idx]) = (s3_tmp);
  ((s4)[s4_idx]) = (s4_tmp);
  ((s6)[s6_idx]) = (s6_tmp);
  ((s5)[s5_idx]) = (s5_tmp);
  ((s7)[s7_idx]) = (s7_tmp);
  (s0_idx) = (((s0_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s2_idx) = (((s2_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s1_idx) = (((s1_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s3_idx) = (((s3_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s4_idx) = (((s4_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s6_idx) = (((s6_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s5_idx) = (((s5_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s7_idx) = (((s7_idx) + ((size_t)(1))) % ((size_t)(1)));
}
