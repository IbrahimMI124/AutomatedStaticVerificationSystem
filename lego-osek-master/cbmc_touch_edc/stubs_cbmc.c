#include <stdint.h>

#include "kernel.h"
#include "ecrobot_interface.h"

// Controlled by harness
uint8_t rv_touch_input;

// ecrobot / nxt stubs: keep deterministic, minimal

void ecrobot_read_bt_packet(U8 *buf, size_t len)
{
  for (size_t i = 0; i < len; i++)
  {
    buf[i] = 0;
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
  return 0;
}

void nxt_motor_set_speed(int port, S32 speed, int brake)
{
  (void)port;
  (void)speed;
  (void)brake;
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

