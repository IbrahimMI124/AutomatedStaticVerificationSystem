#include <stdbool.h>
#include <stdint.h>

#include "deadzone_symmetry_mon.h"

// Copilot externs (read by the generated monitor)
int32_t fr_steering_angle;
int32_t fr_motor_left;
int32_t fr_motor_right;

// Latched violations for each run
static bool deadzone_symmetry_violation_orig_latched;
static bool deadzone_symmetry_violation_seu_latched;

void deadzone_symmetry_violation_orig(void)
{
  deadzone_symmetry_violation_orig_latched = true;
}

void deadzone_symmetry_violation_seu(void)
{
  deadzone_symmetry_violation_seu_latched = true;
}

void deadzone_symmetry_monitor_reset_orig(void)
{
  deadzone_symmetry_violation_orig_latched = false;
}

void deadzone_symmetry_monitor_reset_seu(void)
{
  deadzone_symmetry_violation_seu_latched = false;
}

bool deadzone_symmetry_monitor_violation_orig(void)
{
  return deadzone_symmetry_violation_orig_latched;
}

bool deadzone_symmetry_monitor_violation_seu(void)
{
  return deadzone_symmetry_violation_seu_latched;
}

void deadzone_symmetry_publish(int32_t steering_angle, int32_t motor_left, int32_t motor_right)
{
  fr_steering_angle = steering_angle;
  fr_motor_left = motor_left;
  fr_motor_right = motor_right;
}
