#ifndef MONITOR_INPUTS_H
#define MONITOR_INPUTS_H

#include <stdbool.h>
#include <stdint.h>

#include "elevator_types.h"

#if FLOORS != 5
#error "ElevatorMon.hs currently assumes FLOORS == 5"
#endif

/*
 * These globals are the ONLY interface the Copilot-generated monitor uses.
 * They are updated by the harness after each `system_step()`.
 */

extern uint8_t mon_current_floor;
extern uint8_t mon_dir;
extern uint8_t mon_door;

extern bool mon_req0;
extern bool mon_req1;
extern bool mon_req2;
extern bool mon_req3;
extern bool mon_req4;

extern bool mon_served;

void monitor_inputs_update(void);

#endif
