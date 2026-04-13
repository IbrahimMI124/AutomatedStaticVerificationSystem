#include "nondet.h"

#ifdef CBMC

/* CBMC built-ins */
void __CPROVER_assume(_Bool);

bool nondet_bool(void) {
    _Bool b;
    return (bool)b;
}

uint32_t nondet_u32(void) {
    uint32_t v;
    return v;
}

int32_t nondet_i32_range(int32_t lo, int32_t hi) {
    int32_t v;
    __CPROVER_assume(v >= lo);
    __CPROVER_assume(v <= hi);
    return v;
}

float nondet_float_range(float lo, float hi) {
    float v;
    __CPROVER_assume(v >= lo);
    __CPROVER_assume(v <= hi);
    return v;
}

#else

/* Deterministic PRNG (LCG) so normal runs are repeatable. */
static uint32_t prng_state = 1u;

static uint32_t lcg_next(void) {
    prng_state = prng_state * 1103515245u + 12345u;
    return prng_state;
}

bool nondet_bool(void) {
    return (lcg_next() >> 31) & 1u;
}

uint32_t nondet_u32(void) {
    return lcg_next();
}

static float u32_to_unit_float(uint32_t x) {
    /* Map to [0,1]. Avoid 1.0 exactly. */
    const float denom = 4294967296.0f; /* 2^32 */
    return (float)x / denom;
}

int32_t nondet_i32_range(int32_t lo, int32_t hi) {
    if (hi < lo) {
        int32_t tmp = lo;
        lo = hi;
        hi = tmp;
    }
    uint32_t span = (uint32_t)((int64_t)hi - (int64_t)lo + 1);
    uint32_t r = nondet_u32();
    return lo + (int32_t)(r % span);
}

float nondet_float_range(float lo, float hi) {
    if (hi < lo) {
        float tmp = lo;
        lo = hi;
        hi = tmp;
    }
    float u = u32_to_unit_float(nondet_u32());
    return lo + u * (hi - lo);
}

#endif
