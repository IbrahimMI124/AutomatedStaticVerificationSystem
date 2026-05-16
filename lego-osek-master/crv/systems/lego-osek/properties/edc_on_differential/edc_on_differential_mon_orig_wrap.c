// Wrap the generated monitor with unique symbols for the "original" run.
#define step edc_on_differential_step_orig
#define edc_on_differential_violation edc_on_differential_violation_orig
#include "edc_on_differential_mon.c"
