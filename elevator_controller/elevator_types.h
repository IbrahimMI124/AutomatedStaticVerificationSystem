#ifndef ELEVATOR_TYPES_H
#define ELEVATOR_TYPES_H

#include <stdbool.h>

#ifndef FLOORS
#define FLOORS 5
#endif

#if FLOORS < 2
#error "FLOORS must be >= 2"
#endif

typedef enum { IDLE = 0, UP = 1, DOWN = 2 } Direction;
typedef enum { OPEN = 0, CLOSED = 1 } DoorState;

#endif
