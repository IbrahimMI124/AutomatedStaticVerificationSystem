#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "kernel.h"
#include "ecrobot_interface.h"

#include "seu_support.h"

// Monitor glue
void deadzone_symmetry_publish(int32_t steering_angle, int32_t motor_left, int32_t motor_right);
void deadzone_symmetry_monitor_reset_orig(void);
void deadzone_symmetry_monitor_reset_seu(void);
bool deadzone_symmetry_monitor_violation_orig(void);
bool deadzone_symmetry_monitor_violation_seu(void);

void deadzone_symmetry_step_orig(void);
void deadzone_symmetry_step_seu(void);

// Controller-under-test (from nxtgt.c)
void TaskControl(void);

// RV accessors (compiled from nxtgt.c with -DRV_MONITOR)
int8_t rv_get_EDC_flag(void);
void rv_set_EDC_flag(int8_t v);
uint8_t rv_get_touch_sensor_state(void);
void rv_set_touch_sensor_state(uint8_t v);
S32 rv_get_steering_angle(void);
void rv_set_steering_angle(S32 v);

// Provided by shared stubs
extern uint8_t rv_touch_input;
extern int8_t rv_bt_left_input;
extern int8_t rv_bt_right_input;
extern S32 rv_steering_angle_input;

S32 rv_get_motor_left_speed(void);
S32 rv_get_motor_right_speed(void);
void rv_set_motor_left_speed(S32 v);
void rv_set_motor_right_speed(S32 v);

// Required by nxtgt.c (DeclareCounter(SysTimerCnt))
CounterType SysTimerCnt;

#ifdef CBMC
uint8_t nondet_u8(void);
int8_t nondet_s8(void);
int32_t nondet_i32(void);
#endif

#ifndef STEPS
#define STEPS 6
#endif

#define STEERING_LIMIT 40
#define EDC_OFF 1

// SEU targets
#define SEU_STEERING_ANGLE 1
#define SEU_MOTOR_LEFT 2
#define SEU_MOTOR_RIGHT 3
#define SEU_BT_LEFT 4

#ifndef SEU_TARGET
#define SEU_TARGET SEU_STEERING_ANGLE
#endif

static void controller_init_from_snapshot(int8_t edc_flag_init, uint8_t touch_state_init)
{
  rv_set_EDC_flag(edc_flag_init);
  rv_set_touch_sensor_state(touch_state_init);
}

static int8_t flip_s8_bit(int8_t v, uint8_t bit)
{
  uint8_t uv = (uint8_t)v;
  uv ^= (uint8_t)(1u << (bit & 7u));
  return (int8_t)uv;
}

static int32_t flip_s32_bit(int32_t v, uint8_t bit)
{
  uint32_t uv = (uint32_t)v;
  uv ^= (uint32_t)(1u << (bit & 31u));
  return (int32_t)uv;
}

static void run_original(const uint8_t touch_seq[STEPS],
                         const int8_t bt_left_seq[STEPS],
                         const int32_t steering_angle_seq[STEPS],
                         bool violation_seq[STEPS])
{
  deadzone_symmetry_monitor_reset_orig();

  for (unsigned t = 0; t < STEPS; t++)
  {
    rv_touch_input = touch_seq[t];
    rv_bt_left_input = bt_left_seq[t];
    rv_bt_right_input = 0;
    rv_steering_angle_input = (S32)steering_angle_seq[t];

    TaskControl();

    int32_t steering_angle = (int32_t)rv_get_steering_angle();
    int32_t motor_left = (int32_t)rv_get_motor_left_speed();
    int32_t motor_right = (int32_t)rv_get_motor_right_speed();

    deadzone_symmetry_publish(steering_angle, motor_left, motor_right);
    deadzone_symmetry_step_orig();

    violation_seq[t] = deadzone_symmetry_monitor_violation_orig();
  }
}

static void run_seu(const uint8_t touch_seq[STEPS],
                    const int8_t bt_left_seq[STEPS],
                    const int32_t steering_angle_seq[STEPS],
                    bool violation_seq[STEPS])
{
  deadzone_symmetry_monitor_reset_seu();

#ifdef FORCE_CEX
  uint8_t seu_step = 1u;
  uint8_t seu_bit = 0u;
#else
  uint8_t seu_step = nondet_u8();
  uint8_t seu_bit = nondet_u8();
#ifdef CBMC
  __CPROVER_assume(STEPS >= 2);
  __CPROVER_assume(seu_step > 0u);
  __CPROVER_assume(seu_step < STEPS);
  __CPROVER_assume(seu_bit < 32u);
#endif
#endif

  for (unsigned t = 0; t < STEPS; t++)
  {
#if SEU_TARGET == SEU_STEERING_ANGLE
    rv_steering_angle_input = (t == seu_step) ? flip_s32_bit(steering_angle_seq[t], seu_bit) : (S32)steering_angle_seq[t];
#else
    rv_steering_angle_input = (S32)steering_angle_seq[t];
#endif

#if SEU_TARGET == SEU_BT_LEFT
  rv_bt_left_input = (t == seu_step) ? flip_s8_bit(bt_left_seq[t], seu_bit) : bt_left_seq[t];
#else
  rv_bt_left_input = bt_left_seq[t];
#endif

  rv_bt_right_input = 0;
  rv_touch_input = touch_seq[t];

    TaskControl();

#if SEU_TARGET == SEU_MOTOR_LEFT
    if (t == seu_step)
    {
      rv_set_motor_left_speed(flip_s32_bit(rv_get_motor_left_speed(), seu_bit));
    }
#endif
#if SEU_TARGET == SEU_MOTOR_RIGHT
    if (t == seu_step)
    {
      rv_set_motor_right_speed(flip_s32_bit(rv_get_motor_right_speed(), seu_bit));
    }
#endif

    int32_t steering_angle = (int32_t)rv_get_steering_angle();
    int32_t motor_left = (int32_t)rv_get_motor_left_speed();
    int32_t motor_right = (int32_t)rv_get_motor_right_speed();

    deadzone_symmetry_publish(steering_angle, motor_left, motor_right);
    deadzone_symmetry_step_seu();

    violation_seq[t] = deadzone_symmetry_monitor_violation_seu();
  }
}

int main(void)
{
  int8_t edc_flag_init = (int8_t)EDC_OFF;
  uint8_t touch_state_init = 0u;

  bool violation_seq_orig[STEPS];
  bool violation_seq_seu[STEPS];

  uint8_t touch_seq[STEPS];
  int8_t bt_left_seq[STEPS];
  int32_t steering_angle_seq[STEPS];

  for (unsigned t = 0; t < STEPS; t++)
  {
#ifdef CBMC
    touch_seq[t] = nondet_u8();
    __CPROVER_assume(touch_seq[t] <= 1u);

    bt_left_seq[t] = nondet_s8();
    __CPROVER_assume(bt_left_seq[t] >= -100 && bt_left_seq[t] <= 100);

    steering_angle_seq[t] = nondet_i32();
    __CPROVER_assume(steering_angle_seq[t] >= -STEERING_LIMIT && steering_angle_seq[t] <= STEERING_LIMIT);
#else
    touch_seq[t] = 0u;
    bt_left_seq[t] = 0;
    steering_angle_seq[t] = 0;
#endif
  }

  controller_init_from_snapshot(edc_flag_init, touch_state_init);
  run_original(touch_seq, bt_left_seq, steering_angle_seq, violation_seq_orig);

  controller_init_from_snapshot(edc_flag_init, touch_state_init);
  run_seu(touch_seq, bt_left_seq, steering_angle_seq, violation_seq_seu);

#ifdef CBMC
  for (unsigned t = 0; t < STEPS; t++)
  {
    __CPROVER_assert(violation_seq_orig[t] == violation_seq_seu[t],
                    "CRV check (timestep): SUCCESS => variable not CRV; ASSERTION FAILURE => variable is CRV");
  }
#else
  for (unsigned t = 0; t < STEPS; t++)
  {
    assert(violation_seq_orig[t] == violation_seq_seu[t]);
  }
#endif

  return 0;
}
