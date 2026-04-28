#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "kernel.h"

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

static int8_t flip_s8_bit0(int8_t v)
{
  return (int8_t)(((uint8_t)v) ^ 1u);
}

static void controller_init_known_good(void)
{
  rv_set_EDC_flag((int8_t)1); // EDC_OFF
  rv_set_touch_sensor_state(0u);
}

int main(void)
{
  enum { STEPS = 4 };

  // Hardcode a steady touch input: never pressed
  uint8_t touch_seq[STEPS] = {0u, 0u, 0u, 0u};

  // Original execution
  controller_init_known_good();
  p7_monitor_reset_orig();
  for (unsigned t = 0; t < STEPS; t++)
  {
    rv_touch_input = touch_seq[t];
    TaskControl();
    p7_publish(false, rv_get_EDC_flag());
    touch_edc_step_orig();
  }
  bool violation_original = p7_monitor_violation_orig();

  // SEU execution: flip EDC_flag at step 1 (after the monitor's ignored first sample)
  controller_init_known_good();
  p7_monitor_reset_seu();
  for (unsigned t = 0; t < STEPS; t++)
  {
    rv_touch_input = touch_seq[t];
    if (t == 1u)
    {
      rv_set_EDC_flag(flip_s8_bit0(rv_get_EDC_flag()));
    }
    TaskControl();
    p7_publish(false, rv_get_EDC_flag());
    touch_edc_step_seu();
  }
  bool violation_seu = p7_monitor_violation_seu();

#ifdef CBMC
  __CPROVER_assert(!violation_original, "Sanity: original should not violate P7 on steady touch");
  __CPROVER_assert(violation_seu, "Sanity: EDC_flag flip under steady touch should violate P7");
#else
  assert(!violation_original);
  assert(violation_seu);
#endif

  return 0;
}
