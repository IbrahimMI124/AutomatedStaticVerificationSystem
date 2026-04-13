#ifndef RV_SYSTEMS_NONDET_H
#define RV_SYSTEMS_NONDET_H

#include <stdbool.h>
#include <stdint.h>

/*
 * Nondeterminism / pseudo-random shim.
 *
 * - Under CBMC: values are nondeterministic (uninitialised locals) and range
 *   helpers constrain them via __CPROVER_assume.
 * - Under normal compilation: deterministic PRNG so runs are repeatable.
 */

bool nondet_bool(void);
uint32_t nondet_u32(void);

int32_t nondet_i32_range(int32_t lo, int32_t hi);
float nondet_float_range(float lo, float hi);

#endif
