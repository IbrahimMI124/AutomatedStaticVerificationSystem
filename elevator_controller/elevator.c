#include <assert.h>
#include <stdbool.h>

#include "elevator.h"
#include "nondet.h"

int current_floor = 0;
Direction dir = IDLE;
DoorState door = CLOSED;
bool requests[FLOORS];
bool served_this_step = false;

static bool has_request(void) {
    for (int i = 0; i < FLOORS; i++) {
        if (requests[i]) {
            return true;
        }
    }
    return false;
}

static void update_direction(void) {
    if (!has_request()) {
        dir = IDLE;
        return;
    }

    for (int i = current_floor + 1; i < FLOORS; i++) {
        if (requests[i]) {
            dir = UP;
            return;
        }
    }

    for (int i = current_floor - 1; i >= 0; i--) {
        if (requests[i]) {
            dir = DOWN;
            return;
        }
    }

    dir = IDLE;
}

static void move_one_floor(void) {
    if (dir == UP) {
        if (current_floor < FLOORS - 1) {
            current_floor++;
        }
    } else if (dir == DOWN) {
        if (current_floor > 0) {
            current_floor--;
        }
    }
}

static void serve_current_floor(void) {
    if (requests[current_floor]) {
        served_this_step = true;
        door = OPEN;
        requests[current_floor] = false;
    } else {
        served_this_step = false;
        door = CLOSED;
    }
}

void elevator_init(void) {
    current_floor = 0;
    dir = IDLE;
    door = CLOSED;
    served_this_step = false;
    for (int i = 0; i < FLOORS; i++) {
        requests[i] = false;
    }

}

void system_step(void) {
    served_this_step = false;

    /* If the door was open, close it and consume the whole tick (no movement). */
    if (door == OPEN) {
        door = CLOSED;
        dir = IDLE;
        return;
    }

    /* Nondeterministic new requests: pending requests persist. */
    for (int i = 0; i < FLOORS; i++) {
        if (nondet_bool()) {
            requests[i] = true;
        }
    }

    /* Enforce bounds as an internal safety invariant. */
    assert(current_floor >= 0);
    assert(current_floor < FLOORS);

    update_direction();
    move_one_floor();
    serve_current_floor();

    assert(current_floor >= 0);
    assert(current_floor < FLOORS);
}
