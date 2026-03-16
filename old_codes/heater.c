#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "heater_types.h"
#include "heater.h"

static uint8_t temperature_cpy;

static bool heaton_0_guard(void) {
  return ((((float)(temperature_cpy)) * (((float)(150.0f)) / ((float)(255.0f)))) - ((float)(50.0f))) < ((float)(18.0f));
}

static float heaton_0_arg0(void) {
  return (((float)(temperature_cpy)) * (((float)(150.0f)) / ((float)(255.0f)))) - ((float)(50.0f));
}

static bool heatoff_1_guard(void) {
  return ((((float)(temperature_cpy)) * (((float)(150.0f)) / ((float)(255.0f)))) - ((float)(50.0f))) > ((float)(21.0f));
}

static float heatoff_1_arg0(void) {
  return (((float)(temperature_cpy)) * (((float)(150.0f)) / ((float)(255.0f)))) - ((float)(50.0f));
}

void step(void) {
  float heaton_0_arg_temp0;
  float heatoff_1_arg_temp0;
  (temperature_cpy) = (temperature);
  if ((heaton_0_guard)()) {
    {(heaton_0_arg_temp0) = ((heaton_0_arg0)());
     (heaton)((heaton_0_arg_temp0));}
  };
  if ((heatoff_1_guard)()) {
    {(heatoff_1_arg_temp0) = ((heatoff_1_arg0)());
     (heatoff)((heatoff_1_arg_temp0));}
  };
}
