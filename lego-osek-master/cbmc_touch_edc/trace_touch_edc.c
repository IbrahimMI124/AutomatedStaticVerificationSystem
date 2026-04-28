#include <stdbool.h>
#include <stdint.h>

#include "touch_edc_mon.h"

// Copilot externs (read by the generated monitor)
bool fr_touch;
int8_t fr_edc_flag;

// Latched violations for each run
static bool p7_violation_orig;
static bool p7_violation_seu;

void touch_edc_violation_orig(void)
{
  p7_violation_orig = true;
}

void touch_edc_violation_seu(void)
{
  p7_violation_seu = true;
}

void p7_monitor_reset_orig(void)
{
  p7_violation_orig = false;
}

void p7_monitor_reset_seu(void)
{
  p7_violation_seu = false;
}

bool p7_monitor_violation_orig(void)
{
  return p7_violation_orig;
}

bool p7_monitor_violation_seu(void)
{
  return p7_violation_seu;
}

void p7_publish(bool touch_pressed, int8_t edc_flag)
{
  fr_touch = touch_pressed;
  fr_edc_flag = edc_flag;
}
