/********
* ec2c version 0.67
* c file generated for node : obst_detector 
* context   method = STATIC
* ext call  method = PROCEDURES
********/
/* This program needs external declarations */
#define _obst_detector_EC2C_SRC_FILE
#include "obst_detector.h"
/*--------
 * the following ``constants'' must be defined:
extern _real white_threshold;
extern _real black_threshold;
extern _real obstacle_threshold;
--------*/
/*--------
Internal structure for the call
--------*/
typedef struct  {
   
   //INPUTS
   _real _Co;
   _real _Cg;
   //OUTPUTS
   _boolean _obstacle;
   //REGISTERS
   _boolean M13;
   _boolean M13_nil;
   _boolean M32;
   _boolean M32_nil;
   _boolean M27;
   _boolean M27_nil;
   _boolean M20;
   _boolean M20_nil;
   _boolean M10;
   _boolean M10_nil;
   _boolean M7;
} obst_detector_ctx;

// Single Static Context
static obst_detector_ctx ctx;
/*--------
Output procedures must be defined,
Input procedures must be used:
--------*/
void obst_detector_I_Co(_real V){
   ctx._Co = V;
}
void obst_detector_I_Cg(_real V){
   ctx._Cg = V;
}
extern void obst_detector_O_obstacle(_boolean);
#ifdef CKCHECK
extern void obst_detector_BOT_obstacle();
#endif
/*--------
Internal reset input procedure
--------*/
static void obst_detector_reset_input(){
   //NOTHING FOR THIS VERSION...
}
/*--------
Reset procedure
--------*/
void obst_detector_reset(){
   ctx.M13_nil = _true;
   ctx.M32_nil = _true;
   ctx.M27_nil = _true;
   ctx.M20_nil = _true;
   ctx.M10_nil = _true;
   ctx.M7 = _true;
   obst_detector_reset_input();
}
/*--------
Step procedure
--------*/
void obst_detector_step(){
//LOCAL VARIABLES
   _boolean L4;
   _boolean L6;
   _boolean L3;
   _boolean L12;
   _boolean L11;
   _boolean L2;
   _boolean L17;
   _boolean L19;
   _boolean L16;
   _boolean L15;
   _boolean L24;
   _boolean L26;
   _boolean L23;
   _boolean L22;
   _boolean L30;
   _boolean L31;
   _boolean L29;
   _boolean L28;
   _boolean L21;
   _boolean L14;
   _boolean T13;
   _boolean T32;
   _boolean T27;
   _boolean T20;
   _boolean T10;
//CODE
   L4 = (ctx._Co < obstacle_threshold);
   if (ctx.M7) {
      L6 = _false;
   } else {
      L6 = ctx.M10;
   }
   L3 = (L4 || L6);
   if (ctx.M7) {
      L12 = _false;
   } else {
      L12 = ctx.M13;
   }
   L11 = (! L12);
   L2 = (L3 && L11);
   obst_detector_O_obstacle(L2);
   L17 = (ctx._Cg >= white_threshold);
   if (ctx.M7) {
      L19 = _false;
   } else {
      L19 = ctx.M20;
   }
   L16 = (L17 || L19);
   L15 = (L16 && L11);
   L24 = (ctx._Cg <= black_threshold);
   if (ctx.M7) {
      L26 = _false;
   } else {
      L26 = ctx.M27;
   }
   L23 = (L24 || L26);
   L22 = (L23 && L11);
   L30 = (ctx._Cg <= white_threshold);
   if (ctx.M7) {
      L31 = _false;
   } else {
      L31 = ctx.M32;
   }
   L29 = (L30 || L31);
   L28 = (L29 && L11);
   L21 = (L22 && L28);
   L14 = (L15 && L21);
   T13 = L14;
   T32 = L28;
   T27 = L22;
   T20 = L15;
   T10 = L2;
   ctx.M13 = T13;
   ctx.M13_nil = _false;
   ctx.M32 = T32;
   ctx.M32_nil = _false;
   ctx.M27 = T27;
   ctx.M27_nil = _false;
   ctx.M20 = T20;
   ctx.M20_nil = _false;
   ctx.M10 = T10;
   ctx.M10_nil = _false;
   ctx.M7 = ctx.M7 && !(_true);
}
