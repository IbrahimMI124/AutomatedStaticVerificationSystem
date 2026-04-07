#include "nondet.h"


#ifdef CBMC

bool nondet_bool(void) {
    _Bool b;
    return (bool)b;
}

#else

/* Deterministic PRNG (LCG) so `gcc` runs are repeatable. */
static unsigned int prng_state = 1u;

static unsigned int lcg_next(void) {
    prng_state = prng_state * 1103515245u + 12345u;
    return prng_state;
}

bool nondet_bool(void) {
    return (lcg_next() >> 31) & 1u;
}

#endif
