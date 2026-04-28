/********
* ec2c version 0.67
* context   method = STATIC
* ext call  method = PROCEDURES
* c header file generated for node : Controller 
* to be used with : Controller.c 
********/
#ifndef _Controller_EC2C_H_FILE
#define _Controller_EC2C_H_FILE
/* This program needs external declarations */
#include "Controller_ext.h"
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
//MODULE: Controller 3 2
//IN: _real Cg
//IN: _real Cd
//IN: _real Co
//OUT: _real u_d
//OUT: _real u_g
#ifndef _Controller_EC2C_SRC_FILE
/*-------- Input procedures -------------*/
extern void Controller_I_Cg(_real);
extern void Controller_I_Cd(_real);
extern void Controller_I_Co(_real);
/*-------- Reset procedure -----------*/
extern void Controller_reset();
/*--------Context init = context reset --------*/
#define Controller_init Controller_reset
/*-------- Step procedure -----------*/
extern void Controller_step();
#endif
#endif
