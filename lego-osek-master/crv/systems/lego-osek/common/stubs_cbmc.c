#include <stdint.h>

#include "kernel.h"
#include "ecrobot_interface.h"

// Controlled by harness
uint8_t rv_touch_input;
int8_t rv_bt_left_input;
int8_t rv_bt_right_input;
S32 rv_steering_angle_input;

// Captured motor commands for monitors/harnesses
static S32 rv_motor_steering_speed;
static S32 rv_motor_left_speed;
static S32 rv_motor_right_speed;

S32 rv_get_motor_steering_speed(void) { return rv_motor_steering_speed; }
S32 rv_get_motor_left_speed(void) { return rv_motor_left_speed; }
S32 rv_get_motor_right_speed(void) { return rv_motor_right_speed; }

void rv_set_motor_steering_speed(S32 v) { rv_motor_steering_speed = v; }
void rv_set_motor_left_speed(S32 v) { rv_motor_left_speed = v; }
void rv_set_motor_right_speed(S32 v) { rv_motor_right_speed = v; }

// ecrobot / nxt stubs: keep deterministic, minimal

void ecrobot_read_bt_packet(U8 *buf, size_t len)
{
  for (size_t i = 0; i < len; i++)
  {
    buf[i] = 0;
  }

  if (len > 0)
  {
    buf[0] = (U8)rv_bt_left_input;
  }
  if (len > 1)
  {
    buf[1] = (U8)rv_bt_right_input;
  }
}

U8 ecrobot_get_touch_sensor(int port)
{
  (void)port;
  return (U8)(rv_touch_input ? 1u : 0u);
}

S32 nxt_motor_get_count(int port)
{
  (void)port;
  return rv_steering_angle_input;
}

void nxt_motor_set_speed(int port, S32 speed, int brake)
{
  (void)port;
  (void)speed;
  (void)brake;

  if (port == NXT_PORT_A)
  {
    rv_motor_steering_speed = speed;
    return;
  }
  if (port == NXT_PORT_B)
  {
    rv_motor_left_speed = speed;
    return;
  }
  if (port == NXT_PORT_C)
  {
    rv_motor_right_speed = speed;
    return;
  }
}

void nxt_motor_set_count(int port, S32 cnt)
{
  (void)port;
  (void)cnt;
}

void ecrobot_bt_data_logger(S8 a, S8 b)
{
  (void)a;
  (void)b;
}

void ecrobot_status_monitor(const char *msg)
{
  (void)msg;
}

void ecrobot_set_light_sensor_active(int port) { (void)port; }
void ecrobot_set_light_sensor_inactive(int port) { (void)port; }
void ecrobot_init_sonar_sensor(int port) { (void)port; }
void ecrobot_term_sonar_sensor(int port) { (void)port; }
void ecrobot_init_bt_connection(void) {}
void ecrobot_term_bt_connection(void) {}
S32 ecrobot_get_sonar_sensor(int port) { (void)port; return 0; }

// OSEK-ish stubs
StatusType SignalCounter(int counter)
{
  (void)counter;
  return E_OK;
}

void ShutdownOS(StatusType e)
{
  (void)e;
}

