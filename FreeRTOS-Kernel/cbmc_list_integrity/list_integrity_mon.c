#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "list_integrity_mon_types.h"
#include "list_integrity_mon.h"

static bool fr_list_ok_cpy;

static bool list_integrity_0_guard(void) {
  return !(fr_list_ok_cpy);
}

void step(void) {
  (fr_list_ok_cpy) = (fr_list_ok);
  if ((list_integrity_0_guard)()) {
    {(list_integrity)();}
  };
}
