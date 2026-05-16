#include <stdbool.h>
#include <stdint.h>

#include "steering_sign_mon.h"

// Copilot externs (read by the generated monitor)
int32_t fr_steering_angle;
int32_t fr_cmd_steer;
int32_t fr_motor_steer;

// Latched violations for each run
static bool steering_sign_violation_orig_latched;
static bool steering_sign_violation_seu_latched;

void steering_sign_violation_orig(void)
{
  steering_sign_violation_orig_latched = true;
}

void steering_sign_violation_seu(void)
{
  steering_sign_violation_seu_latched = true;
}

void steering_sign_monitor_reset_orig(void)
{
  steering_sign_violation_orig_latched = false;
}

void steering_sign_monitor_reset_seu(void)
{
  steering_sign_violation_seu_latched = false;
}

bool steering_sign_monitor_violation_orig(void)
{
  return steering_sign_violation_orig_latched;
}

bool steering_sign_monitor_violation_seu(void)
{
  return steering_sign_violation_seu_latched;
}

void steering_sign_publish(int32_t steering_angle, int32_t cmd_steer, int32_t motor_steer)
{
  fr_steering_angle = steering_angle;
  fr_cmd_steer = cmd_steer;
  fr_motor_steer = motor_steer;
}
