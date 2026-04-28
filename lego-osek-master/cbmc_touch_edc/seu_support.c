#include "seu_support.h"

static bool seu_done;

void seu_reset(void)
{
  seu_done = false;
}

uint8_t seu_maybe_flip_u8(uint8_t v)
{
#ifdef CBMC
  if (!seu_done && nondet_bool())
  {
    unsigned int bit = nondet_uint() % 8u;
    seu_done = true;
    return (uint8_t)(v ^ (uint8_t)(1u << bit));
  }
#else
  (void)v;
#endif
  return v;
}

int8_t seu_maybe_flip_s8(int8_t v)
{
#ifdef CBMC
  if (!seu_done && nondet_bool())
  {
    unsigned int bit = nondet_uint() % 8u;
    seu_done = true;
    return (int8_t)(((uint8_t)v) ^ (uint8_t)(1u << bit));
  }
#else
  (void)v;
#endif
  return v;
}
