#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "elevator_mon_types.h"
#include "elevator_mon.h"

static uint8_t mon_door_cpy;
static uint8_t mon_current_floor_cpy;
static uint8_t mon_dir_cpy;
static bool mon_req0_cpy;
static bool mon_req1_cpy;
static bool mon_req2_cpy;
static bool mon_req3_cpy;
static bool mon_req4_cpy;
static bool mon_served_cpy;
static uint8_t s0[(1)] = {((uint8_t)(1))};
static uint8_t s1[(1)] = {((uint8_t)(0))};
static uint8_t s2[(1)] = {((uint8_t)(0))};
static bool s4[(1)] = {(false)};
static uint8_t s3[(1)] = {((uint8_t)(0))};
static bool s6[(1)] = {(false)};
static uint8_t s5[(1)] = {((uint8_t)(0))};
static bool s8[(1)] = {(false)};
static uint8_t s7[(1)] = {((uint8_t)(0))};
static bool s10[(1)] = {(false)};
static uint8_t s9[(1)] = {((uint8_t)(0))};
static bool s12[(1)] = {(false)};
static uint8_t s11[(1)] = {((uint8_t)(0))};
static bool s14[(1)] = {(false)};
static uint8_t s13[(1)] = {((uint8_t)(0))};
static bool s16[(1)] = {(false)};
static uint8_t s15[(1)] = {((uint8_t)(0))};
static bool s18[(1)] = {(false)};
static uint8_t s17[(1)] = {((uint8_t)(0))};
static bool s20[(1)] = {(false)};
static uint8_t s19[(1)] = {((uint8_t)(0))};
static bool s22[(1)] = {(false)};
static uint8_t s21[(1)] = {((uint8_t)(0))};
static size_t s0_idx = (0);
static size_t s1_idx = (0);
static size_t s2_idx = (0);
static size_t s4_idx = (0);
static size_t s3_idx = (0);
static size_t s6_idx = (0);
static size_t s5_idx = (0);
static size_t s8_idx = (0);
static size_t s7_idx = (0);
static size_t s10_idx = (0);
static size_t s9_idx = (0);
static size_t s12_idx = (0);
static size_t s11_idx = (0);
static size_t s14_idx = (0);
static size_t s13_idx = (0);
static size_t s16_idx = (0);
static size_t s15_idx = (0);
static size_t s18_idx = (0);
static size_t s17_idx = (0);
static size_t s20_idx = (0);
static size_t s19_idx = (0);
static size_t s22_idx = (0);
static size_t s21_idx = (0);

static uint8_t s0_get(size_t x) {
  return (s0)[((s0_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s1_get(size_t x) {
  return (s1)[((s1_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s2_get(size_t x) {
  return (s2)[((s2_idx) + (x)) % ((size_t)(1))];
}

static bool s4_get(size_t x) {
  return (s4)[((s4_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s3_get(size_t x) {
  return (s3)[((s3_idx) + (x)) % ((size_t)(1))];
}

static bool s6_get(size_t x) {
  return (s6)[((s6_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s5_get(size_t x) {
  return (s5)[((s5_idx) + (x)) % ((size_t)(1))];
}

static bool s8_get(size_t x) {
  return (s8)[((s8_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s7_get(size_t x) {
  return (s7)[((s7_idx) + (x)) % ((size_t)(1))];
}

static bool s10_get(size_t x) {
  return (s10)[((s10_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s9_get(size_t x) {
  return (s9)[((s9_idx) + (x)) % ((size_t)(1))];
}

static bool s12_get(size_t x) {
  return (s12)[((s12_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s11_get(size_t x) {
  return (s11)[((s11_idx) + (x)) % ((size_t)(1))];
}

static bool s14_get(size_t x) {
  return (s14)[((s14_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s13_get(size_t x) {
  return (s13)[((s13_idx) + (x)) % ((size_t)(1))];
}

static bool s16_get(size_t x) {
  return (s16)[((s16_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s15_get(size_t x) {
  return (s15)[((s15_idx) + (x)) % ((size_t)(1))];
}

static bool s18_get(size_t x) {
  return (s18)[((s18_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s17_get(size_t x) {
  return (s17)[((s17_idx) + (x)) % ((size_t)(1))];
}

static bool s20_get(size_t x) {
  return (s20)[((s20_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s19_get(size_t x) {
  return (s19)[((s19_idx) + (x)) % ((size_t)(1))];
}

static bool s22_get(size_t x) {
  return (s22)[((s22_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s21_get(size_t x) {
  return (s21)[((s21_idx) + (x)) % ((size_t)(1))];
}

static uint8_t s0_gen(void) {
  return mon_door_cpy;
}

static uint8_t s1_gen(void) {
  return mon_current_floor_cpy;
}

static uint8_t s2_gen(void) {
  return mon_dir_cpy;
}

static bool s4_gen(void) {
  return mon_req0_cpy;
}

static uint8_t s3_gen(void) {
  return ((mon_req0_cpy) && (!((s4_get)((0))))) ? (uint8_t)(5) : ((((mon_door_cpy) == ((uint8_t)(0))) && ((mon_current_floor_cpy) == ((uint8_t)(0)))) ? (uint8_t)(0) : ((((s3_get)((0))) > ((uint8_t)(0))) ? ((s3_get)((0))) - ((uint8_t)(1)) : ((uint8_t)(0))));
}

static bool s6_gen(void) {
  return mon_req1_cpy;
}

static uint8_t s5_gen(void) {
  return ((mon_req1_cpy) && (!((s6_get)((0))))) ? (uint8_t)(5) : ((((mon_door_cpy) == ((uint8_t)(0))) && ((mon_current_floor_cpy) == ((uint8_t)(1)))) ? (uint8_t)(0) : ((((s5_get)((0))) > ((uint8_t)(0))) ? ((s5_get)((0))) - ((uint8_t)(1)) : ((uint8_t)(0))));
}

static bool s8_gen(void) {
  return mon_req2_cpy;
}

static uint8_t s7_gen(void) {
  return ((mon_req2_cpy) && (!((s8_get)((0))))) ? (uint8_t)(5) : ((((mon_door_cpy) == ((uint8_t)(0))) && ((mon_current_floor_cpy) == ((uint8_t)(2)))) ? (uint8_t)(0) : ((((s7_get)((0))) > ((uint8_t)(0))) ? ((s7_get)((0))) - ((uint8_t)(1)) : ((uint8_t)(0))));
}

static bool s10_gen(void) {
  return mon_req3_cpy;
}

static uint8_t s9_gen(void) {
  return ((mon_req3_cpy) && (!((s10_get)((0))))) ? (uint8_t)(5) : ((((mon_door_cpy) == ((uint8_t)(0))) && ((mon_current_floor_cpy) == ((uint8_t)(3)))) ? (uint8_t)(0) : ((((s9_get)((0))) > ((uint8_t)(0))) ? ((s9_get)((0))) - ((uint8_t)(1)) : ((uint8_t)(0))));
}

static bool s12_gen(void) {
  return mon_req4_cpy;
}

static uint8_t s11_gen(void) {
  return ((mon_req4_cpy) && (!((s12_get)((0))))) ? (uint8_t)(5) : ((((mon_door_cpy) == ((uint8_t)(0))) && ((mon_current_floor_cpy) == ((uint8_t)(4)))) ? (uint8_t)(0) : ((((s11_get)((0))) > ((uint8_t)(0))) ? ((s11_get)((0))) - ((uint8_t)(1)) : ((uint8_t)(0))));
}

static bool s14_gen(void) {
  return mon_req0_cpy;
}

static uint8_t s13_gen(void) {
  return ((mon_req0_cpy) && (!((s14_get)((0))))) ? (uint8_t)(15) : ((!(mon_req0_cpy)) ? (uint8_t)(0) : ((((s13_get)((0))) > ((uint8_t)(0))) ? ((s13_get)((0))) - ((uint8_t)(1)) : ((uint8_t)(0))));
}

static bool s16_gen(void) {
  return mon_req1_cpy;
}

static uint8_t s15_gen(void) {
  return ((mon_req1_cpy) && (!((s16_get)((0))))) ? (uint8_t)(15) : ((!(mon_req1_cpy)) ? (uint8_t)(0) : ((((s15_get)((0))) > ((uint8_t)(0))) ? ((s15_get)((0))) - ((uint8_t)(1)) : ((uint8_t)(0))));
}

static bool s18_gen(void) {
  return mon_req2_cpy;
}

static uint8_t s17_gen(void) {
  return ((mon_req2_cpy) && (!((s18_get)((0))))) ? (uint8_t)(15) : ((!(mon_req2_cpy)) ? (uint8_t)(0) : ((((s17_get)((0))) > ((uint8_t)(0))) ? ((s17_get)((0))) - ((uint8_t)(1)) : ((uint8_t)(0))));
}

static bool s20_gen(void) {
  return mon_req3_cpy;
}

static uint8_t s19_gen(void) {
  return ((mon_req3_cpy) && (!((s20_get)((0))))) ? (uint8_t)(15) : ((!(mon_req3_cpy)) ? (uint8_t)(0) : ((((s19_get)((0))) > ((uint8_t)(0))) ? ((s19_get)((0))) - ((uint8_t)(1)) : ((uint8_t)(0))));
}

static bool s22_gen(void) {
  return mon_req4_cpy;
}

static uint8_t s21_gen(void) {
  return ((mon_req4_cpy) && (!((s22_get)((0))))) ? (uint8_t)(15) : ((!(mon_req4_cpy)) ? (uint8_t)(0) : ((((s21_get)((0))) > ((uint8_t)(0))) ? ((s21_get)((0))) - ((uint8_t)(1)) : ((uint8_t)(0))));
}

static bool viol_move_with_open_door_0_guard(void) {
  return !((!(((s0_get)((0))) == ((uint8_t)(0)))) || ((mon_current_floor_cpy) == ((s1_get)((0)))));
}

static bool viol_floor_bounds_1_guard(void) {
  return !((mon_current_floor_cpy) < ((uint8_t)(5)));
}

static bool viol_door_open_only_when_serving_2_guard(void) {
  return !((!((mon_door_cpy) == ((uint8_t)(0)))) || (mon_served_cpy));
}

static bool viol_dir_consistent_up_3_guard(void) {
  return !((!(((s2_get)((0))) == ((uint8_t)(1)))) || ((mon_current_floor_cpy) >= ((s1_get)((0)))));
}

static bool viol_dir_consistent_down_4_guard(void) {
  return !((!(((s2_get)((0))) == ((uint8_t)(2)))) || ((mon_current_floor_cpy) <= ((s1_get)((0)))));
}

static bool viol_no_skip_up_5_guard(void) {
  return !((!(((s2_get)((0))) == ((uint8_t)(1)))) || (((mon_current_floor_cpy) == ((s1_get)((0)))) || ((mon_current_floor_cpy) == (((s1_get)((0))) + ((uint8_t)(1))))));
}

static bool viol_no_skip_down_6_guard(void) {
  return !((!(((s2_get)((0))) == ((uint8_t)(2)))) || (((mon_current_floor_cpy) == ((s1_get)((0)))) || (((mon_current_floor_cpy) + ((uint8_t)(1))) == ((s1_get)((0))))));
}

static bool viol_idle_correctness_7_guard(void) {
  return !((!(((mon_door_cpy) == ((uint8_t)(1))) && (!((mon_req0_cpy) || ((mon_req1_cpy) || ((mon_req2_cpy) || ((mon_req3_cpy) || (mon_req4_cpy)))))))) || ((mon_dir_cpy) == ((uint8_t)(0))));
}

static bool viol_req_not_served_0_8_guard(void) {
  return (((s3_get)((0))) == ((uint8_t)(0))) && ((mon_req0_cpy) && ((!((mon_req0_cpy) && (!((s4_get)((0)))))) && (!(((mon_door_cpy) == ((uint8_t)(0))) && ((mon_current_floor_cpy) == ((uint8_t)(0)))))));
}

static bool viol_req_not_served_1_9_guard(void) {
  return (((s5_get)((0))) == ((uint8_t)(0))) && ((mon_req1_cpy) && ((!((mon_req1_cpy) && (!((s6_get)((0)))))) && (!(((mon_door_cpy) == ((uint8_t)(0))) && ((mon_current_floor_cpy) == ((uint8_t)(1)))))));
}

static bool viol_req_not_served_2_10_guard(void) {
  return (((s7_get)((0))) == ((uint8_t)(0))) && ((mon_req2_cpy) && ((!((mon_req2_cpy) && (!((s8_get)((0)))))) && (!(((mon_door_cpy) == ((uint8_t)(0))) && ((mon_current_floor_cpy) == ((uint8_t)(2)))))));
}

static bool viol_req_not_served_3_11_guard(void) {
  return (((s9_get)((0))) == ((uint8_t)(0))) && ((mon_req3_cpy) && ((!((mon_req3_cpy) && (!((s10_get)((0)))))) && (!(((mon_door_cpy) == ((uint8_t)(0))) && ((mon_current_floor_cpy) == ((uint8_t)(3)))))));
}

static bool viol_req_not_served_4_12_guard(void) {
  return (((s11_get)((0))) == ((uint8_t)(0))) && ((mon_req4_cpy) && ((!((mon_req4_cpy) && (!((s12_get)((0)))))) && (!(((mon_door_cpy) == ((uint8_t)(0))) && ((mon_current_floor_cpy) == ((uint8_t)(4)))))));
}

static bool viol_starvation_0_13_guard(void) {
  return (((s13_get)((0))) == ((uint8_t)(0))) && ((mon_req0_cpy) && ((!((mon_req0_cpy) && (!((s14_get)((0)))))) && (!(!(mon_req0_cpy)))));
}

static bool viol_starvation_1_14_guard(void) {
  return (((s15_get)((0))) == ((uint8_t)(0))) && ((mon_req1_cpy) && ((!((mon_req1_cpy) && (!((s16_get)((0)))))) && (!(!(mon_req1_cpy)))));
}

static bool viol_starvation_2_15_guard(void) {
  return (((s17_get)((0))) == ((uint8_t)(0))) && ((mon_req2_cpy) && ((!((mon_req2_cpy) && (!((s18_get)((0)))))) && (!(!(mon_req2_cpy)))));
}

static bool viol_starvation_3_16_guard(void) {
  return (((s19_get)((0))) == ((uint8_t)(0))) && ((mon_req3_cpy) && ((!((mon_req3_cpy) && (!((s20_get)((0)))))) && (!(!(mon_req3_cpy)))));
}

static bool viol_starvation_4_17_guard(void) {
  return (((s21_get)((0))) == ((uint8_t)(0))) && ((mon_req4_cpy) && ((!((mon_req4_cpy) && (!((s22_get)((0)))))) && (!(!(mon_req4_cpy)))));
}

void step(void) {
  uint8_t s0_tmp;
  uint8_t s1_tmp;
  uint8_t s2_tmp;
  bool s4_tmp;
  uint8_t s3_tmp;
  bool s6_tmp;
  uint8_t s5_tmp;
  bool s8_tmp;
  uint8_t s7_tmp;
  bool s10_tmp;
  uint8_t s9_tmp;
  bool s12_tmp;
  uint8_t s11_tmp;
  bool s14_tmp;
  uint8_t s13_tmp;
  bool s16_tmp;
  uint8_t s15_tmp;
  bool s18_tmp;
  uint8_t s17_tmp;
  bool s20_tmp;
  uint8_t s19_tmp;
  bool s22_tmp;
  uint8_t s21_tmp;
  (mon_door_cpy) = (mon_door);
  (mon_current_floor_cpy) = (mon_current_floor);
  (mon_dir_cpy) = (mon_dir);
  (mon_req0_cpy) = (mon_req0);
  (mon_req1_cpy) = (mon_req1);
  (mon_req2_cpy) = (mon_req2);
  (mon_req3_cpy) = (mon_req3);
  (mon_req4_cpy) = (mon_req4);
  (mon_served_cpy) = (mon_served);
  if ((viol_move_with_open_door_0_guard)()) {
    {(viol_move_with_open_door)();}
  };
  if ((viol_floor_bounds_1_guard)()) {
    {(viol_floor_bounds)();}
  };
  if ((viol_door_open_only_when_serving_2_guard)()) {
    {(viol_door_open_only_when_serving)();}
  };
  if ((viol_dir_consistent_up_3_guard)()) {
    {(viol_dir_consistent_up)();}
  };
  if ((viol_dir_consistent_down_4_guard)()) {
    {(viol_dir_consistent_down)();}
  };
  if ((viol_no_skip_up_5_guard)()) {
    {(viol_no_skip_up)();}
  };
  if ((viol_no_skip_down_6_guard)()) {
    {(viol_no_skip_down)();}
  };
  if ((viol_idle_correctness_7_guard)()) {
    {(viol_idle_correctness)();}
  };
  if ((viol_req_not_served_0_8_guard)()) {
    {(viol_req_not_served_0)();}
  };
  if ((viol_req_not_served_1_9_guard)()) {
    {(viol_req_not_served_1)();}
  };
  if ((viol_req_not_served_2_10_guard)()) {
    {(viol_req_not_served_2)();}
  };
  if ((viol_req_not_served_3_11_guard)()) {
    {(viol_req_not_served_3)();}
  };
  if ((viol_req_not_served_4_12_guard)()) {
    {(viol_req_not_served_4)();}
  };
  if ((viol_starvation_0_13_guard)()) {
    {(viol_starvation_0)();}
  };
  if ((viol_starvation_1_14_guard)()) {
    {(viol_starvation_1)();}
  };
  if ((viol_starvation_2_15_guard)()) {
    {(viol_starvation_2)();}
  };
  if ((viol_starvation_3_16_guard)()) {
    {(viol_starvation_3)();}
  };
  if ((viol_starvation_4_17_guard)()) {
    {(viol_starvation_4)();}
  };
  (s0_tmp) = ((s0_gen)());
  (s1_tmp) = ((s1_gen)());
  (s2_tmp) = ((s2_gen)());
  (s4_tmp) = ((s4_gen)());
  (s3_tmp) = ((s3_gen)());
  (s6_tmp) = ((s6_gen)());
  (s5_tmp) = ((s5_gen)());
  (s8_tmp) = ((s8_gen)());
  (s7_tmp) = ((s7_gen)());
  (s10_tmp) = ((s10_gen)());
  (s9_tmp) = ((s9_gen)());
  (s12_tmp) = ((s12_gen)());
  (s11_tmp) = ((s11_gen)());
  (s14_tmp) = ((s14_gen)());
  (s13_tmp) = ((s13_gen)());
  (s16_tmp) = ((s16_gen)());
  (s15_tmp) = ((s15_gen)());
  (s18_tmp) = ((s18_gen)());
  (s17_tmp) = ((s17_gen)());
  (s20_tmp) = ((s20_gen)());
  (s19_tmp) = ((s19_gen)());
  (s22_tmp) = ((s22_gen)());
  (s21_tmp) = ((s21_gen)());
  ((s0)[s0_idx]) = (s0_tmp);
  ((s1)[s1_idx]) = (s1_tmp);
  ((s2)[s2_idx]) = (s2_tmp);
  ((s4)[s4_idx]) = (s4_tmp);
  ((s3)[s3_idx]) = (s3_tmp);
  ((s6)[s6_idx]) = (s6_tmp);
  ((s5)[s5_idx]) = (s5_tmp);
  ((s8)[s8_idx]) = (s8_tmp);
  ((s7)[s7_idx]) = (s7_tmp);
  ((s10)[s10_idx]) = (s10_tmp);
  ((s9)[s9_idx]) = (s9_tmp);
  ((s12)[s12_idx]) = (s12_tmp);
  ((s11)[s11_idx]) = (s11_tmp);
  ((s14)[s14_idx]) = (s14_tmp);
  ((s13)[s13_idx]) = (s13_tmp);
  ((s16)[s16_idx]) = (s16_tmp);
  ((s15)[s15_idx]) = (s15_tmp);
  ((s18)[s18_idx]) = (s18_tmp);
  ((s17)[s17_idx]) = (s17_tmp);
  ((s20)[s20_idx]) = (s20_tmp);
  ((s19)[s19_idx]) = (s19_tmp);
  ((s22)[s22_idx]) = (s22_tmp);
  ((s21)[s21_idx]) = (s21_tmp);
  (s0_idx) = (((s0_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s1_idx) = (((s1_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s2_idx) = (((s2_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s4_idx) = (((s4_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s3_idx) = (((s3_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s6_idx) = (((s6_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s5_idx) = (((s5_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s8_idx) = (((s8_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s7_idx) = (((s7_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s10_idx) = (((s10_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s9_idx) = (((s9_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s12_idx) = (((s12_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s11_idx) = (((s11_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s14_idx) = (((s14_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s13_idx) = (((s13_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s16_idx) = (((s16_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s15_idx) = (((s15_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s18_idx) = (((s18_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s17_idx) = (((s17_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s20_idx) = (((s20_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s19_idx) = (((s19_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s22_idx) = (((s22_idx) + ((size_t)(1))) % ((size_t)(1)));
  (s21_idx) = (((s21_idx) + ((size_t)(1))) % ((size_t)(1)));
}
