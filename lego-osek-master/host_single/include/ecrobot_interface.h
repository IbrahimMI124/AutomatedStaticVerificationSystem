#ifndef HOST_SINGLE_ECROBOT_INTERFACE_H
#define HOST_SINGLE_ECROBOT_INTERFACE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int8_t S8;
typedef uint8_t U8;
typedef int32_t S32;

enum {
    NXT_PORT_A = 0,
    NXT_PORT_B = 1,
    NXT_PORT_C = 2,
    NXT_PORT_S1 = 0,
    NXT_PORT_S2 = 1,
    NXT_PORT_S3 = 2,
    NXT_PORT_S4 = 3,
};

void ecrobot_set_light_sensor_active(int port);
void ecrobot_set_light_sensor_inactive(int port);
void ecrobot_init_sonar_sensor(int port);
void ecrobot_term_sonar_sensor(int port);
void ecrobot_init_bt_connection(void);
void ecrobot_term_bt_connection(void);

void ecrobot_read_bt_packet(U8 *buf, size_t len);
void ecrobot_bt_data_logger(S8 speed, S8 steer);

U8 ecrobot_get_touch_sensor(int port);
S32 ecrobot_get_sonar_sensor(int port);

void ecrobot_status_monitor(const char *title);

void nxt_motor_set_speed(int port, S32 speed, int brake);
void nxt_motor_set_count(int port, S32 count);
S32 nxt_motor_get_count(int port);

#ifdef __cplusplus
}
#endif

#endif
