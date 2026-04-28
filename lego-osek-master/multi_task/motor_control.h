/********
* ec2c version 0.67
* context   method = STATIC
* ext call  method = PROCEDURES
* c header file generated for node : motor_control 
* to be used with : motor_control.c 
********/
#ifndef _motor_control_EC2C_H_FILE
#define _motor_control_EC2C_H_FILE
/* This program needs external declarations */
#include "motor_control_ext.h"
/*-------- Predefined types ---------*/
#ifndef _EC2C_PREDEF_TYPES
#define _EC2C_PREDEF_TYPES
typedef int _boolean;
typedef int _integer;
typedef char* _string;
typedef double _real;
typedef double _double;
typedef float _float;
#define _false 0
#define _true 1
#endif
/*--------- Pragmas ----------------*/
//MODULE: motor_control 2 2
//IN: _real Cg
//IN: _real Cd
//OUT: _real u_d
//OUT: _real u_g
#ifndef _motor_control_EC2C_SRC_FILE
/*-------- Input procedures -------------*/
extern void motor_control_I_Cg(_real);
extern void motor_control_I_Cd(_real);
/*-------- Reset procedure -----------*/
extern void motor_control_reset();
/*--------Context init = context reset --------*/
#define motor_control_init motor_control_reset
/*-------- Step procedure -----------*/
extern void motor_control_step();
#endif
#endif
