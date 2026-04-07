#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <stdbool.h>
#include "elevator_types.h"

/* Elevator state exposed as globals for monitor integration. */
extern int current_floor;
extern Direction dir;
extern DoorState door;
extern bool requests[FLOORS];

/* True only on the step where a request is served (door opens). */
extern bool served_this_step;

void elevator_init(void);
void system_step(void);

#endif
