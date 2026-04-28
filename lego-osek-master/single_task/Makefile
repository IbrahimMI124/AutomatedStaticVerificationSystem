## application's name
TARGET=ctrl_single

## folders where C files are located
USR_PATH=.

## link options
USR_LIBS=

## user files
USR_CFILES=\
   my_glue.c \
   Controller.c \

## generic makefile for OSEK (requires MDL2LUS2OSEK variable)
include $(MDL2LUS2OSEK)/generic.mak

Controller.c: Controller.lus
	lus2c Controller.lus Controller -ctx-static

Controller.lus: matlab/ExRobotAndEnvironmentController.mdl
	mdl2lus matlab/ExRobotAndEnvironmentController.mdl -system Controller

clear: clean
	rm -fr *.xml Controller.lus Controller.c Controller.h *.ec *.mws build
