#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "kernel.h"
#include "ecrobot_interface.h"

#include "seu_support.h"

// Monitor glue
void p7_publish(bool touch_pressed, int8_t edc_flag);
void p7_monitor_reset_orig(void);
void p7_monitor_reset_seu(void);
bool p7_monitor_violation_orig(void);
bool p7_monitor_violation_seu(void);

void touch_edc_step_orig(void);
void touch_edc_step_seu(void);

// Controller-under-test (from nxtgt.c)
void TaskControl(void);

// RV accessors (compiled from nxtgt.c with -DRV_MONITOR)
int8_t rv_get_EDC_flag(void);
void rv_set_EDC_flag(int8_t v);
uint8_t rv_get_touch_sensor_state(void);
void rv_set_touch_sensor_state(uint8_t v);

// Provided by stubs
extern uint8_t rv_touch_input;

// Required by nxtgt.c (DeclareCounter(SysTimerCnt))
CounterType SysTimerCnt;

#ifdef CBMC
uint8_t nondet_u8(void);
#endif

#ifndef STEPS
#define STEPS 6
#endif

// SEU targets
#define SEU_TOUCH_INPUT 1
#define SEU_EDC_FLAG 2
#define SEU_TOUCH_STATE 3
#define SEU_STEERING_ANGLE 4

#ifndef SEU_TARGET
#define SEU_TARGET SEU_TOUCH_INPUT
#endif

S32 rv_get_steering_angle(void);
void rv_set_steering_angle(S32 v);

static void controller_init_from_snapshot(int8_t edc_flag_init, uint8_t touch_state_init)
{
  rv_set_EDC_flag(edc_flag_init);
  rv_set_touch_sensor_state(touch_state_init);
}

static uint8_t flip_u8_bit(uint8_t v, uint8_t bit)
{
  return (uint8_t)(v ^ (uint8_t)(1u << (bit & 7u)));
}

static int8_t flip_s8_bit(int8_t v, uint8_t bit)
{
  uint8_t uv = (uint8_t)v;
  uv ^= (uint8_t)(1u << (bit & 7u));
  return (int8_t)uv;
}

static void run_original(const uint8_t touch_seq[STEPS])
{
  p7_monitor_reset_orig();

  for (unsigned t = 0; t < STEPS; t++)
  {
    rv_touch_input = touch_seq[t];
    TaskControl();

    bool touch_pressed = (rv_touch_input != 0);
    int8_t edc_flag = rv_get_EDC_flag();

    p7_publish(touch_pressed, edc_flag);
    touch_edc_step_orig();
  }
}

static void run_seu(const uint8_t touch_seq[STEPS])
{
  p7_monitor_reset_seu();

  // Choose exactly one injection point after the first sample.
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
  __CPROVER_assume(seu_bit < 8u);
#endif
#endif

  for (unsigned t = 0; t < STEPS; t++)
  {
#if SEU_TARGET == SEU_TOUCH_INPUT
    rv_touch_input = (t == seu_step) ? flip_u8_bit(touch_seq[t], seu_bit) : touch_seq[t];
#else
    rv_touch_input = touch_seq[t];
#endif

#if SEU_TARGET == SEU_EDC_FLAG
    if (t == seu_step)
    {
      rv_set_EDC_flag(flip_s8_bit(rv_get_EDC_flag(), seu_bit));
    }
#endif
#if SEU_TARGET == SEU_TOUCH_STATE
    if (t == seu_step)
    {
      rv_set_touch_sensor_state(flip_u8_bit(rv_get_touch_sensor_state(), seu_bit));
    }
#endif

#if SEU_TARGET == SEU_STEERING_ANGLE
if (t == seu_step) {
    // Use a new flip_s32_bit helper or cast to u32
    uint32_t val = (uint32_t)rv_get_steering_angle();
    val ^= (1u << (seu_bit % 32)); 
    rv_set_steering_angle((S32)val);
}
#endif


    TaskControl();

    bool touch_pressed = (rv_touch_input != 0);
    int8_t edc_flag = rv_get_EDC_flag();

    p7_publish(touch_pressed, edc_flag);
    touch_edc_step_seu();
  }
}

int main(void)
{
  // Fixed initialization (same for both runs)
  int8_t edc_flag_init = (int8_t)1;   // EDC_OFF
  uint8_t touch_state_init = 0u;

  uint8_t touch_seq[STEPS];
#ifdef FORCE_CEX
  for (unsigned t = 0; t < STEPS; t++)
  {
    touch_seq[t] = 0u;
  }
#else
  for (unsigned t = 0; t < STEPS; t++)
  {
    touch_seq[t] = nondet_u8();
#ifdef CBMC
    __CPROVER_assume(touch_seq[t] <= 1u);
#endif
  }
#endif

  // Original execution
  controller_init_from_snapshot(edc_flag_init, touch_state_init);
  run_original(touch_seq);
  int violation_original = p7_monitor_violation_orig();

  // SEU execution (same init + same inputs, but with a single-bit upset)
  controller_init_from_snapshot(edc_flag_init, touch_state_init);
  run_seu(touch_seq);
  int violation_seu = p7_monitor_violation_seu();

#ifdef FORCE_CEX
#ifdef CBMC
  __CPROVER_assert(violation_original == 0,
                  "FORCE_CEX: expected original run to have no violation");
  __CPROVER_assert(violation_seu != 0,
                  "FORCE_CEX: expected SEU run to violate P7");
#else
  assert(violation_original == 0);
  assert(violation_seu != 0);
#endif
#endif

#ifdef CBMC
  __CPROVER_assert(violation_original == violation_seu,
                  "CRV check: SUCCESS => variable not CRV; ASSERTION FAILURE => variable is CRV");
#else
  assert(violation_original == violation_seu);
#endif

  return 0;
}
