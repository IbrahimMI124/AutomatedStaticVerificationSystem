#include "ecrobot_interface.h"

#include <stdio.h>
#include <string.h>

static S32 g_motor_speed[3];
static S32 g_motor_count[3];
static U8 g_touch = 0;
static S32 g_sonar = 42;

void ecrobot_set_light_sensor_active(int port) { (void)port; }
void ecrobot_set_light_sensor_inactive(int port) { (void)port; }
void ecrobot_init_sonar_sensor(int port) { (void)port; }
void ecrobot_term_sonar_sensor(int port) { (void)port; }
void ecrobot_init_bt_connection(void) {}
void ecrobot_term_bt_connection(void) {}

void ecrobot_read_bt_packet(U8 *buf, size_t len)
{
    if(!buf || len == 0) return;
    memset(buf, 0, len);

    static int t = 0;
    t++;

    /* byte0 speed, byte1 steer */
    int speed = (t % 200) - 100;
    int steer = ((t * 3) % 200) - 100;

    buf[0] = (U8)(S8)speed;
    if(len > 1) buf[1] = (U8)(S8)steer;

    /* toggle touch occasionally */
    if((t % 250) == 0) g_touch = (U8)(g_touch ? 0 : 1);

    /* fake sonar */
    g_sonar = 20 + (t % 80);
}

void ecrobot_bt_data_logger(S8 speed, S8 steer)
{
    (void)speed;
    (void)steer;
}

U8 ecrobot_get_touch_sensor(int port)
{
    (void)port;
    return g_touch;
}

S32 ecrobot_get_sonar_sensor(int port)
{
    (void)port;
    return g_sonar;
}

void ecrobot_status_monitor(const char *title)
{
    printf("%s\n", title ? title : "(null)");
}

void nxt_motor_set_speed(int port, S32 speed, int brake)
{
    (void)brake;
    if(port < 0 || port >= 3) return;
    g_motor_speed[port] = speed;

    /* crude integration to make steering angle change */
    g_motor_count[port] += speed / 10;
}

void nxt_motor_set_count(int port, S32 count)
{
    if(port < 0 || port >= 3) return;
    g_motor_count[port] = count;
}

S32 nxt_motor_get_count(int port)
{
    if(port < 0 || port >= 3) return 0;
    return g_motor_count[port];
}
