#include "kernel.h"
#include "periods.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include <string.h>  /* a cause du strlen plus bas */
#include "motor_control.h"
#include "obst_detector.h"

#define RIGHT_WHEEL_PORT NXT_PORT_A
#define LEFT_WHEEL_PORT NXT_PORT_B
#define RIGHT_LIGHT_SENSOR NXT_PORT_S1
#define LEFT_LIGHT_SENSOR NXT_PORT_S4
#define SONAR_SENSOR NXT_PORT_S2

//empirical constants used to adjust motor output
#define V_AVG 1.2
#define V_MAX 3.5

// from 0-255 to 0-100
#define LIMIT_SONAR(X) ((X)/2.56)

/***** OSEK : NE PAS MODIFIER */
DeclareCounter(SysTimerCnt);
DeclareResource(lcd);
DeclareTask(LowTask);
DeclareTask(HighTask);

/* LEJOS OSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void)
{
	StatusType ercd; /* Increment OSEK Alarm System Timer Count */
	ercd = SignalCounter( SysTimerCnt );
	if( ercd != E_OK ){
		ShutdownOS( ercd );
	}
}
/******* FIN OSEK */

/* Initialisation et finalisations OSEK */
void ecrobot_device_initialize() {
	ecrobot_init_sonar_sensor(SONAR_SENSOR);
	ecrobot_set_light_sensor_active(RIGHT_LIGHT_SENSOR);
	ecrobot_set_light_sensor_active(LEFT_LIGHT_SENSOR);

	motor_control_reset();
	obst_detector_reset();
}

void ecrobot_device_terminate() {
	ecrobot_set_light_sensor_inactive(RIGHT_LIGHT_SENSOR);
	ecrobot_set_light_sensor_inactive(LEFT_LIGHT_SENSOR);
	ecrobot_term_sonar_sensor(SONAR_SENSOR);

	ecrobot_set_motor_speed(RIGHT_WHEEL_PORT, 0);
	ecrobot_set_motor_speed(LEFT_WHEEL_PORT, 0);
}

/*------------------------------
	Déclarations
--------------------------------
On met typiquement ici les déclarations
de variables partagées, de fonctions
accessoires etc.
------------------------------*/

static int sens_white_cal_right;
static int sens_white_cal_left;
static int sens_black_cal_right;
static int sens_black_cal_left;
static _boolean multi_task_obst = 0;

/* utilitaire : affichage d'une variable.
L'affichage DOIT être réalisé en exclusion
mutuelle.
D'où l'utilisation du verrou "lcd"
fourni par le noyau temps réel (cf. kernel_cfg.h)
*/
void show_var(char* varname, int line, int value) {
	GetResource(lcd);
	int l;
	display_goto_xy(0, line);
	display_string(varname);
	l = strlen(varname);
	display_goto_xy(l, line);
	display_int(value, 4);
	display_goto_xy(0, line+1);
	display_update();
	ReleaseResource(lcd);
}

/* utilitaire : affichage d'une string */
void show_string(char* varname, int line) {
	GetResource(lcd);
	display_goto_xy(0, line);
	display_string(varname);
	display_goto_xy(0, line+1);
	display_update();
	ReleaseResource(lcd);
}

/* utilitaire : nettoyage de l'écran */
void reset_lcd() {
	GetResource(lcd);
	for (int i = 0; i < 10; i++) {
		display_goto_xy(0, i);
		display_string("                ");
	}
	display_update();
	ReleaseResource(lcd);
}

/*------------------------------
	Initialisations
--------------------------------
On doit se débrouiller pour que NOS initialisations
soient effectuées. Comme on ne veut PAS TOUCHER
une seule ligne au code OSEK, on utilise un kernel_cfg.c
modifié, où une nouvelle fonction, "usr_init()", est
appelée à l'initialisation du système.
CETTE FONCTION DOIT ÊTRE DÉFINIE
(vide éventuellement)
------------------------------*/
void usr_init(void) {
	GetResource(lcd);

	show_string("White calib", 0);
	while (!ecrobot_is_ENTER_button_pressed());
	sens_white_cal_right = ecrobot_get_light_sensor(RIGHT_LIGHT_SENSOR);
	sens_white_cal_left = ecrobot_get_light_sensor(LEFT_LIGHT_SENSOR);

	systick_wait_ms(1000);

	show_string("Black calib", 0);
	while (!ecrobot_is_ENTER_button_pressed());
	sens_black_cal_right = ecrobot_get_light_sensor(RIGHT_LIGHT_SENSOR);
	sens_black_cal_left = ecrobot_get_light_sensor(LEFT_LIGHT_SENSOR);

	reset_lcd();
	show_string("Calib completed", 0);
	show_var("white_right", 1, sens_white_cal_right);
	show_var("white_left", 2, sens_white_cal_left);
	show_var("black_right", 3, sens_black_cal_right);
	show_var("black_left", 4, sens_black_cal_left);

	systick_wait_ms(1000);
	while (!ecrobot_is_ENTER_button_pressed());
	reset_lcd();

	ReleaseResource(lcd);
}

/* Limits the PWM value sent to the servo motor
between -100 and 100 (limits defined by the API)
and actually controls the motor with this value
by calling ecrobot_set_motor_speed

Parameters
	port: NXT_PORT_A, NXT_PORT_B, NXT_PORT_C
	speed : theorically between -100 and 100

Returns
	None 
*/ 
void output_motor(U32 port, _real speed) {
	int speed_percent;

	if (speed > 100.0) {
		speed_percent = 100;
	} else if (speed < -100.0) {
		speed_percent = -100;
	} else {
		speed_percent = (int)speed;
	}

	ecrobot_set_motor_speed(port, speed_percent); // brake mode = 0 (float?)
}

/* Receives the output of the controller and
do the necessary empirical adjustements with
V_MAX and V_AVG in order to give a smoother
movement to the robot. Only sends the control
command to the motor if not in obstacle mode

Parameters
	ud: right output value of the motor_control subsytem

Returns
	None 
*/ 
void motor_control_O_u_d(_real ud) {
	show_var("ud", 5, ud * 100);

	ud += V_AVG;
	_real Pd = ((ud/V_MAX) * 100) + 10;

	show_var("Pd", 3, Pd);

	if (!multi_task_obst) output_motor(RIGHT_WHEEL_PORT, Pd);
}

/* Receives the output of the motor controller and
do the necessary empirical adjustements with
V_MAX and V_AVG in order to give a smoother
movement to the robot. Only sends the control
command to the motor if not in obstacle mode

Parameters
	ug: left output value of the motor_control subsytem

Returns
	None 
*/
void motor_control_O_u_g(_real ug) {
	show_var("ug", 6, ug * 100);

	ug += V_AVG;
	_real Pg = ((ug/V_MAX) * 100) + 10;

	show_var("Pg", 4, Pg);

	if (!multi_task_obst) output_motor(LEFT_WHEEL_PORT, Pg);
}

/* Receives the output of the obstacle controller
and decides what to send to the servo motors so
that the robot avoid the obstacle.

Parameters
	obst: output value of the obst_detector subsytem

Returns
	None 
*/
void obst_detector_O_obstacle(_boolean obst) {
	_real Pg = -32;
	_real Pd = 25;

	multi_task_obst = obst;

	if (multi_task_obst) {
		show_var("Pd", 3, Pg);
		show_var("Pg", 4, Pd);
		output_motor(LEFT_WHEEL_PORT, Pg);
		output_motor(RIGHT_WHEEL_PORT, Pd);
	} 
}

/*------------------------------
  Definitions des taches
--------------------------------
Elles doivent :
- être déclarées par la macro TASK
- le code doit se terminer par la macro TerminateTask()
------------------------------*/

/* High Task: motor control

Reads the light sensor values, uses the calibration
to adjust them between 0 and 100 and finally calls 
the motor_control subsystem sending these values
as parameters.
*/
TASK(HighTask) {
	U16 raw_sensor_right;
	U16 raw_sensor_left;

	raw_sensor_right = ecrobot_get_light_sensor(RIGHT_LIGHT_SENSOR);
	raw_sensor_left = ecrobot_get_light_sensor(LEFT_LIGHT_SENSOR);

	if (raw_sensor_right < sens_white_cal_right) {
		raw_sensor_right = sens_white_cal_right;
	} else if (raw_sensor_right > sens_black_cal_right) {
		raw_sensor_right = sens_black_cal_right;
	}

	if (raw_sensor_left < sens_white_cal_left) {
		raw_sensor_left = sens_white_cal_left;
	} else if (raw_sensor_left > sens_black_cal_left) {
		raw_sensor_left = sens_black_cal_left;
	}

	raw_sensor_right = ((raw_sensor_right - sens_white_cal_right) * 100 /
				(sens_black_cal_right - sens_white_cal_right));

	raw_sensor_left = ((raw_sensor_left - sens_white_cal_left) * 100 /
				(sens_black_cal_left - sens_white_cal_left));

	raw_sensor_right = 100 - raw_sensor_right;
	raw_sensor_left = 100 - raw_sensor_left;

	show_var("raw_right", 0, raw_sensor_right);
	show_var("raw_left", 1, raw_sensor_left);

	motor_control_I_Cd(raw_sensor_right);
	motor_control_I_Cg(raw_sensor_left);
	motor_control_step();

	TerminateTask();
}

/* Low Task: obstacle control

Reads the sonar sensor value, verify if the sensor
is ready (has not send -1), then limits the read
value between 0-100 and send it to the obst_detector
subsystem, together with a adjusted value of the left
light sensor (the same way as it is done in the High 
Task)
*/
TASK(LowTask) {
	static U32 previous_raw_sonar_value = 255;
	U16 raw_sensor_left;

	raw_sensor_left = ecrobot_get_light_sensor(LEFT_LIGHT_SENSOR);

	if (raw_sensor_left < sens_white_cal_left) {
		raw_sensor_left = sens_white_cal_left;
	} else if (raw_sensor_left > sens_black_cal_left) {
		raw_sensor_left = sens_black_cal_left;
	}

	raw_sensor_left = ((raw_sensor_left - sens_white_cal_left) * 100 /
				(sens_black_cal_left - sens_white_cal_left));

	raw_sensor_left = 100 - raw_sensor_left;
	
	U32 raw_sonar_value = ecrobot_get_sonar_sensor(SONAR_SENSOR);

	if (raw_sonar_value < 0)
		raw_sonar_value = previous_raw_sonar_value;
	else
		previous_raw_sonar_value = raw_sonar_value;

	_float sonar_value = LIMIT_SONAR(raw_sonar_value);
	show_var("sonar", 2, sonar_value);

	obst_detector_I_Co(sonar_value);
	obst_detector_I_Cg(raw_sensor_left);
	obst_detector_step();

	TerminateTask();
}