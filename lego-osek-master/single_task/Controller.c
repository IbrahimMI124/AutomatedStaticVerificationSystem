/********
* ec2c version 0.67
* c file generated for node : Controller 
* context   method = STATIC
* ext call  method = PROCEDURES
********/
/* This program needs external declarations */
#define _Controller_EC2C_SRC_FILE
#include "Controller.h"
/*--------
 * the following ``constants'' must be defined:
extern _real rotate_speed;
extern _real rotate_speed_neg;
extern _real white_treshold;
extern _real black_treshold;
extern _real obstacle_treshold;
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
   _real _Co;
   //OUTPUTS
   _real _u_d;
   _real _u_g;
   //REGISTERS
   _real M54;
   _boolean M54_nil;
   _boolean M15;
   _boolean M15_nil;
   _boolean M34;
   _boolean M34_nil;
   _boolean M29;
   _boolean M29_nil;
   _boolean M22;
   _boolean M22_nil;
   _boolean M10;
   _boolean M10_nil;
   _boolean M7;
} Controller_ctx;

// Single Static Context
static Controller_ctx ctx;
/*--------
Output procedures must be defined,
Input procedures must be used:
--------*/
void Controller_I_Cg(_real V){
   ctx._Cg = V;
}
void Controller_I_Cd(_real V){
   ctx._Cd = V;
}
void Controller_I_Co(_real V){
   ctx._Co = V;
}
extern void Controller_O_u_d(_real);
extern void Controller_O_u_g(_real);
#ifdef CKCHECK
extern void Controller_BOT_u_d();
extern void Controller_BOT_u_g();
#endif
/*--------
Internal reset input procedure
--------*/
static void Controller_reset_input(){
   //NOTHING FOR THIS VERSION...
}
/*--------
Reset procedure
--------*/
void Controller_reset(){
   ctx.M54_nil = _true;
   ctx.M15_nil = _true;
   ctx.M34_nil = _true;
   ctx.M29_nil = _true;
   ctx.M22_nil = _true;
   ctx.M10_nil = _true;
   ctx.M7 = _true;
   Controller_reset_input();
}
/*--------
Step procedure
--------*/
void Controller_step(){
//LOCAL VARIABLES
   _boolean L6;
   _boolean L11;
   _boolean L5;
   _boolean L14;
   _boolean L13;
   _boolean L4;
   _real L37;
   _real L48;
   _real L51;
   _real L47;
   _real L45;
   _real L52;
   _real L44;
   _real L42;
   _real L55;
   _real L41;
   _real L62;
   _boolean L61;
   _real L63;
   _real L60;
   _real L59;
   _real L66;
   _real L64;
   _real L58;
   _real L57;
   _real L40;
   _real L36;
   _real L3;
   _real L72;
   _real L71;
   _real L70;
   _real L68;
   _real T54;
   _boolean L19;
   _boolean L21;
   _boolean L18;
   _boolean L17;
   _boolean L26;
   _boolean L28;
   _boolean L25;
   _boolean L24;
   _boolean L32;
   _boolean L33;
   _boolean L31;
   _boolean L30;
   _boolean L23;
   _boolean L16;
   _boolean T15;
   _boolean T34;
   _boolean T29;
   _boolean T22;
   _boolean T10;
//CODE
   if (ctx.M7) {
      L6 = _false;
   } else {
      L6 = ctx.M10;
   }
   L11 = (ctx._Co < obstacle_treshold);
   L5 = (L6 || L11);
   if (ctx.M7) {
      L14 = _false;
   } else {
      L14 = ctx.M15;
   }
   L13 = (! L14);
   L4 = (L5 && L13);
   L37 = (1.000000 / 2.000000);
   L48 = (pi / 200.000000);
   L51 = (ctx._Cd - ctx._Cg);
   L47 = (L48 * L51);
   L45 = (0.002000 * L47);
   if (ctx.M7) {
      L52 = 0.000000;
   } else {
      L52 = ctx.M54;
   }
   L44 = (L45 + L52);
   L42 = (ki_teta * L44);
   L55 = (kp_teta * L47);
   L41 = (L42 + L55);
   L62 = (2.000000 * L41);
   L61 = (L62 < 0.000000);
   L63 = (- L62);
   if (L61) {
      L60 = L63;
   } else {
      L60 = L62;
   }
   L59 = (- L60);
   L66 = (ctx._Cd + ctx._Cg);
   L64 = (0.005000 * L66);
   L58 = (L59 + L64);
   L57 = (2.000000 * L58);
   L40 = (L41 + L57);
   L36 = (L37 * L40);
   if (L4) {
      L3 = rotate_speed;
   } else {
      L3 = L36;
   }
   Controller_O_u_d(L3);
   L72 = (- L41);
   L71 = (L72 + L57);
   L70 = (L37 * L71);
   if (L4) {
      L68 = rotate_speed_neg;
   } else {
      L68 = L70;
   }
   Controller_O_u_g(L68);
   T54 = L44;
   L19 = (ctx._Cg >= white_treshold);
   if (ctx.M7) {
      L21 = _false;
   } else {
      L21 = ctx.M22;
   }
   L18 = (L19 || L21);
   L17 = (L18 && L13);
   L26 = (ctx._Cg <= black_treshold);
   if (ctx.M7) {
      L28 = _false;
   } else {
      L28 = ctx.M29;
   }
   L25 = (L26 || L28);
   L24 = (L25 && L13);
   L32 = (ctx._Cg <= white_treshold);
   if (ctx.M7) {
      L33 = _false;
   } else {
      L33 = ctx.M34;
   }
   L31 = (L32 || L33);
   L30 = (L31 && L13);
   L23 = (L24 && L30);
   L16 = (L17 && L23);
   T15 = L16;
   T34 = L30;
   T29 = L24;
   T22 = L17;
   T10 = L4;
   ctx.M54 = T54;
   ctx.M54_nil = _false;
   ctx.M15 = T15;
   ctx.M15_nil = _false;
   ctx.M34 = T34;
   ctx.M34_nil = _false;
   ctx.M29 = T29;
   ctx.M29_nil = _false;
   ctx.M22 = T22;
   ctx.M22_nil = _false;
   ctx.M10 = T10;
   ctx.M10_nil = _false;
   ctx.M7 = ctx.M7 && !(_true);
}
