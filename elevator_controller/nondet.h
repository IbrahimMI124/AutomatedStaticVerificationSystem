#ifndef NONDET_H
#define NONDET_H

#include <stdbool.h>

/*
 * Nondeterminism shim.
 * - Under CBMC, we implement `nondet_bool()` in `nondet.c` using a
 *   nondeterministic (uninitialised) _Bool local.
 * - Under normal compilation, `nondet.c` provides a deterministic PRNG.
 */

bool nondet_bool(void);

#endif
