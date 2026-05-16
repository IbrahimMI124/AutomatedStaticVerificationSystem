#include <stdbool.h>
#include <stdint.h>

#include "edc_on_differential_mon.h"

// Copilot externs (read by the generated monitor)
int8_t fr_edc_flag;
int32_t fr_steering_angle;
int32_t fr_cmd_speed;
int32_t fr_motor_left;
int32_t fr_motor_right;

// Latched violations for each run
static bool edc_on_differential_violation_orig_latched;
static bool edc_on_differential_violation_seu_latched;

void edc_on_differential_violation_orig(void)
{
  edc_on_differential_violation_orig_latched = true;
}

void edc_on_differential_violation_seu(void)
{
  edc_on_differential_violation_seu_latched = true;
}

void edc_on_differential_monitor_reset_orig(void)
{
  edc_on_differential_violation_orig_latched = false;
}

void edc_on_differential_monitor_reset_seu(void)
{
  edc_on_differential_violation_seu_latched = false;
}

bool edc_on_differential_monitor_violation_orig(void)
{
  return edc_on_differential_violation_orig_latched;
}

bool edc_on_differential_monitor_violation_seu(void)
{
  return edc_on_differential_violation_seu_latched;
}

void edc_on_differential_publish(int8_t edc_flag,
                                 int32_t steering_angle,
                                 int32_t cmd_speed,
                                 int32_t motor_left,
                                 int32_t motor_right)
{
  fr_edc_flag = edc_flag;
  fr_steering_angle = steering_angle;
  fr_cmd_speed = cmd_speed;
  fr_motor_left = motor_left;
  fr_motor_right = motor_right;
}
