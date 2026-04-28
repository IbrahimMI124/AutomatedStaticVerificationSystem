/********
* ec2c version 0.67
* context   method = STATIC
* ext call  method = PROCEDURES
* c header file generated for node : obst_detector 
* to be used with : obst_detector.c 
********/
#ifndef _obst_detector_EC2C_H_FILE
#define _obst_detector_EC2C_H_FILE
/* This program needs external declarations */
#include "obst_detector_ext.h"
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
//MODULE: obst_detector 2 1
//IN: _real Co
//IN: _real Cg
//OUT: _boolean obstacle
#ifndef _obst_detector_EC2C_SRC_FILE
/*-------- Input procedures -------------*/
extern void obst_detector_I_Co(_real);
extern void obst_detector_I_Cg(_real);
/*-------- Reset procedure -----------*/
extern void obst_detector_reset();
/*--------Context init = context reset --------*/
#define obst_detector_init obst_detector_reset
/*-------- Step procedure -----------*/
extern void obst_detector_step();
#endif
#endif
