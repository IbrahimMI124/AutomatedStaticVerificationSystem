#include "monitor_inputs.h"

#include "elevator.h"

uint8_t mon_current_floor;
uint8_t mon_dir;
uint8_t mon_door;

bool mon_req0;
bool mon_req1;
bool mon_req2;
bool mon_req3;
bool mon_req4;

bool mon_served;

void monitor_inputs_update(void) {
    mon_current_floor = (uint8_t)current_floor;
    mon_dir = (uint8_t)dir;
    mon_door = (uint8_t)door;

    mon_req0 = requests[0];
    mon_req1 = requests[1];
    mon_req2 = requests[2];
    mon_req3 = requests[3];
    mon_req4 = requests[4];

    mon_served = served_this_step;
}
