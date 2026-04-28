#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef CBMC
_Bool nondet_bool(void);
unsigned int nondet_uint(void);
#endif

void seu_reset(void);
uint8_t seu_maybe_flip_u8(uint8_t v);
int8_t seu_maybe_flip_s8(int8_t v);
