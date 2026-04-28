/********
* ec2c version 0.67
* c file generated for node : motor_control 
* context   method = STATIC
* ext call  method = PROCEDURES
********/
/* This program needs external declarations */
#define _motor_control_EC2C_SRC_FILE
#include "motor_control.h"
/*--------
 * the following ``constants'' must be defined:
extern _real pi;
extern _real kp_teta;
extern _real ki_teta;
--------*/
/*--------
Internal structure for the call
--------*/
typedef struct  {
   
   //INPUTS
   _real _Cg;
   _real _Cd;
   //OUTPUTS
   _real _u_d;
   _real _u_g;
   //REGISTERS
   _real M22;
   _boolean M22_nil;
   _boolean M19;
} motor_control_ctx;

// Single Static Context
static motor_control_ctx ctx;
/*--------
Output procedures must be defined,
Input procedures must be used:
--------*/
void motor_control_I_Cg(_real V){
   ctx._Cg = V;
}
void motor_control_I_Cd(_real V){
   ctx._Cd = V;
}
extern void motor_control_O_u_d(_real);
extern void motor_control_O_u_g(_real);
#ifdef CKCHECK
extern void motor_control_BOT_u_d();
extern void motor_control_BOT_u_g();
#endif
/*--------
Internal reset input procedure
--------*/
static void motor_control_reset_input(){
   //NOTHING FOR THIS VERSION...
}
/*--------
Reset procedure
--------*/
void motor_control_reset(){
   ctx.M22_nil = _true;
   ctx.M19 = _true;
   motor_control_reset_input();
}
/*--------
Step procedure
--------*/
void motor_control_step(){
//LOCAL VARIABLES
   _real L3;
   _real L14;
   _real L17;
   _real L13;
   _real L11;
   _real L18;
   _real L10;
   _real L8;
   _real L23;
   _real L7;
   _real L30;
   _boolean L29;
   _real L31;
   _real L28;
   _real L27;
   _real L34;
   _real L32;
   _real L26;
   _real L25;
   _real L6;
   _real L2;
   _real L38;
   _real L37;
   _real L36;
   _real T22;
//CODE
   L3 = (1.000000 / 2.000000);
   L14 = (pi / 200.000000);
   L17 = (ctx._Cd - ctx._Cg);
   L13 = (L14 * L17);
   L11 = (0.002000 * L13);
   if (ctx.M19) {
      L18 = 0.000000;
   } else {
      L18 = ctx.M22;
   }
   L10 = (L11 + L18);
   L8 = (ki_teta * L10);
   L23 = (kp_teta * L13);
   L7 = (L8 + L23);
   L30 = (2.000000 * L7);
   L29 = (L30 < 0.000000);
   L31 = (- L30);
   if (L29) {
      L28 = L31;
   } else {
      L28 = L30;
   }
   L27 = (- L28);
   L34 = (ctx._Cd + ctx._Cg);
   L32 = (0.005000 * L34);
   L26 = (L27 + L32);
   L25 = (2.000000 * L26);
   L6 = (L7 + L25);
   L2 = (L3 * L6);
   motor_control_O_u_d(L2);
   L38 = (- L7);
   L37 = (L38 + L25);
   L36 = (L3 * L37);
   motor_control_O_u_g(L36);
   T22 = L10;
   ctx.M22 = T22;
   ctx.M22_nil = _false;
   ctx.M19 = ctx.M19 && !(_true);
}
