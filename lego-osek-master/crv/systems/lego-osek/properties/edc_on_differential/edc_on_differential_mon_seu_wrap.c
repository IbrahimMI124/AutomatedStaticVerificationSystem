// Wrap the generated monitor with unique symbols for the "SEU" run.
#define step edc_on_differential_step_seu
#define edc_on_differential_violation edc_on_differential_violation_seu
#include "edc_on_differential_mon.c"
