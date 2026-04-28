## application's name
TARGET=ctrl_multi

## folders where C files are located
USR_PATH=.

## link options
USR_LIBS=

## user files
USR_CFILES=\
   motor_control.c \
   obst_detector.c \
   glue_code.c \

## generic makefile for OSEK (requires MDL2LUS2OSEK variable)
include $(MDL2LUS2OSEK)/generic.mak

## Compilation Lustre->C

obst_detector.lus: matlab/ExRobotAndEnvironmentController.mdl
	mdl2lus matlab/ExRobotAndEnvironmentController.mdl -system obst_detector

obst_detector.c: obst_detector.lus
	lus2c obst_detector.lus obst_detector -ctx-static

motor_control.lus: matlab/ExRobotAndEnvironmentController.mdl
	mdl2lus matlab/ExRobotAndEnvironmentController.mdl -system motor_control

motor_control.c: motor_control.lus
	lus2c motor_control.lus motor_control -ctx-static

clear: clean
	rm -rf *.xml *.rxe *.bin *.ec *.mws obst_detector.lus obst_detector.c obst_detector.h motor_control.lus motor_control.c motor_control.h build 
	

