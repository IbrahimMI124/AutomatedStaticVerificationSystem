#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "automotive_system_types.h"
#include "automotive_system.h"

static bool fault_cpy;
static bool safe_mode_cpy;
static bool gas_pressed_cpy;
static bool brake_cpy;
static bool engine_cpy;
static float brake_torque_cpy;
static bool abrupt_gas_cpy;
static float torque_cpy;
static bool acc_cpy;
static float wsl_cpy;
static float wsr_cpy;
static bool stationary_cpy;
static bool collision_imminent_cpy;
static bool rolling_cpy;
static bool brake_request_cpy;
static float gas_angle_cpy;
static bool s1[(1)] = {(false)};
static uint32_t s0[(1)] = {((uint32_t)(0))};
static bool s2[(1)] = {(false)};
static bool s4[(1)] = {(false)};
static uint32_t s3[(1)] = {((uint32_t)(0))};
static bool s5[(1)] = {(false)};
static bool s7[(1)] = {(false)};
static uint32_t s6[(1)] = {((uint32_t)(0))};
static bool s8[(1)] = {(false)};
static bool s10[(1)] = {(false)};
static uint32_t s9[(1)] = {((uint32_t)(0))};
static bool s11[(1)] = {(false)};
static size_t s1_idx = (0);
static size_t s0_idx = (0);
static size_t s2_idx = (0);
static size_t s4_idx = (0);
static size_t s3_idx = (0);
static size_t s5_idx = (0);
static size_t s7_idx = (0);
static size_t s6_idx = (0);
static size_t s8_idx = (0);
static size_t s10_idx = (0);
static size_t s9_idx = (0);
static size_t s11_idx = (0);

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

static bool s7_get(size_t x) {
  return (s7)[((s7_idx) + (x)) % ((size_t)(1))];
}

static uint32_t s6_get(size_t x) {
  return (s6)[((s6_idx) + (x)) % ((size_t)(1))];
}

static bool s8_get(size_t x) {
  return (s8)[((s8_idx) + (x)) % ((size_t)(1))];
}

static bool s10_get(size_t x) {
  return (s10)[((s10_idx) + (x)) % ((size_t)(1))];
}

static uint32_t s9_get(size_t x) {
  return (s9)[((s9_idx) + (x)) % ((size_t)(1))];
}

static bool s11_get(size_t x) {
  return (s11)[((s11_idx) + (x)) % ((size_t)(1))];
}

static bool s1_gen(void) {
  return fault_cpy;
}

static uint32_t s0_gen(void) {
  return ((fault_cpy) && (!((s1_get)((0))))) ? (uint32_t)(3) : ((safe_mode_cpy) ? (uint32_t)(0) : ((((s0_get)((0))) > ((uint32_t)(0))) ? ((s0_get)((0))) - ((uint32_t)(1)) : ((uint32_t)(0))));
}

static bool s2_gen(void) {
  return ((fault_cpy) && (!((s1_get)((0))))) ? true : ((safe_mode_cpy) ? false : ((s2_get)((0))));
}

static bool s4_gen(void) {
  return gas_pressed_cpy;
}

static uint32_t s3_gen(void) {
  return ((gas_pressed_cpy) && (!((s4_get)((0))))) ? (uint32_t)(3) : (((!(brake_cpy)) && (engine_cpy)) ? (uint32_t)(0) : ((((s3_get)((0))) > ((uint32_t)(0))) ? ((s3_get)((0))) - ((uint32_t)(1)) : ((uint32_t)(0))));
}

static bool s5_gen(void) {
  return ((gas_pressed_cpy) && (!((s4_get)((0))))) ? true : (((!(brake_cpy)) && (engine_cpy)) ? false : ((s5_get)((0))));
}

static bool s7_gen(void) {
  return gas_pressed_cpy;
}

static uint32_t s6_gen(void) {
  return ((gas_pressed_cpy) && (!((s7_get)((0))))) ? (uint32_t)(5) : (((brake_torque_cpy) == ((float)(0.0f))) ? (uint32_t)(0) : ((((s6_get)((0))) > ((uint32_t)(0))) ? ((s6_get)((0))) - ((uint32_t)(1)) : ((uint32_t)(0))));
}

static bool s8_gen(void) {
  return ((gas_pressed_cpy) && (!((s7_get)((0))))) ? true : (((brake_torque_cpy) == ((float)(0.0f))) ? false : ((s8_get)((0))));
}

static bool s10_gen(void) {
  return abrupt_gas_cpy;
}

static uint32_t s9_gen(void) {
  return ((abrupt_gas_cpy) && (!((s10_get)((0))))) ? (uint32_t)(5) : (((torque_cpy) == ((float)(0.0f))) ? (uint32_t)(0) : ((((s9_get)((0))) > ((uint32_t)(0))) ? ((s9_get)((0))) - ((uint32_t)(1)) : ((uint32_t)(0))));
}

static bool s11_gen(void) {
  return ((abrupt_gas_cpy) && (!((s10_get)((0))))) ? true : (((torque_cpy) == ((float)(0.0f))) ? false : ((s11_get)((0))));
}

static bool viol_torque_limit_0_guard(void) {
  return !((torque_cpy) <= ((float)(350.0f)));
}

static bool viol_brake_acc_conflict_1_guard(void) {
  return !(!((brake_cpy) && (acc_cpy)));
}

static bool viol_wheel_speed_mismatch_2_guard(void) {
  return !(((((wsl_cpy) - (wsr_cpy)) < ((float)(0.0f))) ? ((float)(0.0f)) - ((wsl_cpy) - (wsr_cpy)) : ((wsl_cpy) - (wsr_cpy))) <= (((float)(0.15f)) * (wsl_cpy)));
}

static bool viol_fault_safe_mode_3_guard(void) {
  return (((s0_get)((0))) == ((uint32_t)(0))) && (((s2_get)((0))) && (!(safe_mode_cpy)));
}

static bool viol_r1_collision_response_4_guard(void) {
  return !((!((stationary_cpy) && (collision_imminent_cpy))) || ((brake_cpy) && (!(engine_cpy))));
}

static bool viol_r2_gas_override_5_guard(void) {
  return (((s3_get)((0))) == ((uint32_t)(0))) && (((s5_get)((0))) && (!((!(brake_cpy)) && (engine_cpy))));
}

static bool viol_ac_engine_mapping_6_guard(void) {
  return !((!(((stationary_cpy) || (rolling_cpy)) && (!(brake_request_cpy)))) || ((engine_cpy) == ((gas_angle_cpy) > ((float)(0.2f)))));
}

static bool viol_gc_brake_torque_zero_7_guard(void) {
  return (((s6_get)((0))) == ((uint32_t)(0))) && (((s8_get)((0))) && (!((brake_torque_cpy) == ((float)(0.0f)))));
}

static bool viol_gf_torque_min_8_guard(void) {
  return (((s9_get)((0))) == ((uint32_t)(0))) && (((s11_get)((0))) && (!((torque_cpy) == ((float)(0.0f)))));
}

void step(void) {
  bool s1_tmp;
  uint32_t s0_tmp;
  bool s2_tmp;
  bool s4_tmp;
  uint32_t s3_tmp;
  bool s5_tmp;
  bool s7_tmp;
  uint32_t s6_tmp;
  bool s8_tmp;
  bool s10_tmp;
  uint32_t s9_tmp;
  bool s11_tmp;
  (fault_cpy) = (fault);
  (safe_mode_cpy) = (safe_mode);
  (gas_pressed_cpy) = (gas_pressed);
  (brake_cpy) = (brake);
  (engine_cpy) = (engine);
  (brake_torque_cpy) = (brake_torque);
  (abrupt_gas_cpy) = (abrupt_gas);
  (torque_cpy) = (torque);
  (acc_cpy) = (acc);
  (wsl_cpy) = (wsl);
  (wsr_cpy) = (wsr);
  (stationary_cpy) = (stationary);
  (collision_imminent_cpy) = (collision_imminent);
  (rolling_cpy) = (rolling);
  (brake_request_cpy) = (brake_request);
  (gas_angle_cpy) = (gas_angle);
  if ((viol_torque_limit_0_guard)()) {
    {(viol_torque_limit)();}
  };
  if ((viol_brake_acc_conflict_1_guard)()) {
    {(viol_brake_acc_conflict)();}
  };
  if ((viol_wheel_speed_mismatch_2_guard)()) {
    {(viol_wheel_speed_mismatch)();}
  };
  if ((viol_fault_safe_mode_3_guard)()) {
    {(viol_fault_safe_mode)();}
  };
  if ((viol_r1_collision_response_4_guard)()) {
    {(viol_r1_collision_response)();}
  };
  if ((viol_r2_gas_override_5_guard)()) {
    {(viol_r2_gas_override)();}
  };
  if ((viol_ac_engine_mapping_6_guard)()) {
    {(viol_ac_engine_mapping)();}
  };
  if ((viol_gc_brake_torque_zero_7_guard)()) {
    {(viol_gc_brake_torque_zero)();}
  };
  if ((viol_gf_torque_min_8_guard)()) {
    {(viol_gf_torque_min)();}
  };
  (s1_tmp) = ((s1_gen)());
  (s0_tmp) = ((s0_gen)());
  (s2_tmp) = ((s2_gen)());
  (s4_tmp) = ((s4_gen)());
  (s3_tmp) = ((s3_gen)());
  (s5_tmp) = ((s5_gen)());
  (s7_tmp) = ((s7_gen)());
  (s6_tmp) = ((s6_gen)());
  (s8_tmp) = ((s8_gen)());
  (s10_tmp) = ((s10_gen)());
  (s9_tmp) = ((s9_gen)());
  (s11_tmp) = ((s11_gen)());
  ((s1)[s1_idx]) = (s1_tmp);
  ((s0)[s0_idx]) = (s0_tmp);
  ((s2)[s2_idx]) = (s2_tmp);
  ((s4)[s4_idx]) = (s4_tmp);
  ((s3)[s3_idx]) = (s3_tmp);
  ((s5)[s5_idx]) = (s5_tmp);
  ((s7)[s7_idx]) = (s7_tmp);
  ((s6)[s6_idx]) = (s6_tmp);
  ((s8)[s8_idx]) = (s8_tmp);
  ((s10)[s10_idx]) = (s10_tmp);
  ((s9)[s9_idx]) = (s9_tmp);
  ((s11)[s11_idx]) = (s11_tmp);
  (s1_idx) = (((s1_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s0_idx) = (((s0_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s2_idx) = (((s2_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s4_idx) = (((s4_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s3_idx) = (((s3_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s5_idx) = (((s5_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s7_idx) = (((s7_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s6_idx) = (((s6_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s8_idx) = (((s8_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s10_idx) = (((s10_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s9_idx) = (((s9_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s11_idx) = (((s11_idx) + ((size_t)(1))) % ((size_t)(1)));
}
