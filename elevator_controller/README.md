
# Elevator Controller + Runtime Monitor (CBMC harness)

This repo contains a tiny elevator controller (`elevator.c`) and a bounded-step harness (`system.c`).
Optionally, you can enable a Copilot-generated runtime monitor (`elevator_mon.c`) and use CBMC to prove (or refute) properties over bounded executions.

The harness also supports *fault injection* via compile-time macros (`INJECT_CASE`, `INJECT_FLOOR`) to deliberately violate specific properties.

## What’s in here

- `elevator.c` / `elevator.h`
	- Controller + environment step (`system_step()`), with nondeterministic requests.
	- State is global: `current_floor`, `dir`, `door`, `requests[]`, `served_this_step`.

- `monitor_inputs.c` / `monitor_inputs.h`
	- Adapter layer: copies controller state into `mon_*` variables once per tick.
	- The monitor reads *only* `mon_*`.

- `ElevatorMon.hs`
	- Copilot spec defining properties P1–P8.

- `elevator_mon.c` / `elevator_mon.h` / `elevator_mon_types.h`
	- Generated monitor implementation from `ElevatorMon.hs`.
	- `step()` evaluates guards and calls the `viol_*` callbacks in `system.c`.

- `nondet.c` / `nondet.h`
	- `nondet_bool()` uses `__CPROVER_nondet_bool()` under `-DCBMC`.
	- Otherwise it uses a small deterministic PRNG for normal runs.

- `system.c`
	- Runs the bounded tick loop.
	- Under `-DWITH_COPILOT`, calls `step()` each tick.
	- Under `-DVERIFY`, turns monitor violations into assertions.
	- Under `-DINJECT_CASE!=0`, mutates the `mon_*` snapshot to force a violation.

## Properties (P1–P8)

These are defined in `ElevatorMon.hs` and reported in `system.c` via `violation_id`:

- **P1**: no movement when the door was open on the previous tick
- **P2**: reported floor is within bounds (`0..4`)
- **P3**: no skipping floors (UP and DOWN variants)
- **P4**: if the door is open, it must be opening due to serving (`mon_served == true`)
- **P5**: a pending request must be served within `kServe = 5` ticks (per floor)
- **P6**: direction consistency (UP must not decrease floor; DOWN must not increase)
- **P7**: if door is closed and there are no pending requests, direction must be IDLE
- **P8**: bounded “no starvation”: a pending request must be cleared within `kStarve = 15` ticks (per floor)

Note: P5/P8 are *bounded response* properties. With unconstrained nondeterministic arrivals, they can fail unless you constrain the environment or use the dedicated injection cases.

## Build & run (plain gcc)

The default `Makefile` builds a simple executable **without** the Copilot monitor:

```bash
cd /home/mohammed-ibrahim/Downloads/Work/PE_RE/Software_Testing/test-system/elevator_controller
make
./elevator
```

## Build & run (gcc + monitor)

To include the runtime monitor:

```bash
gcc -std=c11 -Wall -Wextra -O0 -g -I. \
	-DWITH_COPILOT \
	-o elevator_mon_run \
	system.c elevator.c monitor_inputs.c nondet.c elevator_mon.c

./elevator_mon_run
echo $?
```

Exit code is `0` if no monitor fired, otherwise `1`.

If you want assertion-style failures (useful for CBMC and for crashing fast under gcc):

```bash
gcc -std=c11 -Wall -Wextra -O0 -g -I. \
	-DWITH_COPILOT -DVERIFY \
	-o elevator_mon_verify \
	system.c elevator.c monitor_inputs.c nondet.c elevator_mon.c
```

## CBMC: the one command you’ll reuse

For property checking you typically want:

- `-DWITH_COPILOT` to actually run the monitor (`step()`)
- `-DVERIFY` so violations become `__CPROVER_assert(...)`
- `-DCBMC` so `nondet_bool()` becomes `__CPROVER_nondet_bool()`

Baseline template:

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT \
	--bounds-check --pointer-check \
	--unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

### Loop unwinding rule of thumb

`system.c` runs `MAIN_TICKS` iterations (default `15`). To avoid unwinding assertion failures, use:

- `--unwind MAIN_TICKS+1`

Examples:

- default (`MAIN_TICKS=15`) → `--unwind 16`
- `-DMAIN_TICKS=20` → `--unwind 21`

If you want to ignore unwinding assertions (not recommended for “proved” results):

```bash
cbmc ... --no-unwinding-assertions
```

### Getting a counterexample trace

```bash
cbmc ... --trace --stop-on-fail
```

## Fault injection (INJECT_CASE)

Fault injection is implemented in `system.c` by mutating the `mon_*` snapshot *after* `monitor_inputs_update()` and *before* `step()`.
This means injections target the **monitor’s view** of the system; they do not mutate controller state.

### Injection cases overview

| `INJECT_CASE` | Intended violation |
|---:|---|
| 0 | no injection |
| 1 | P1: movement while door was open (prev tick) |
| 2 | P2: floor out of bounds |
| 3 | P3 (UP): skip floors (+2 jump) |
| 4 | P3 (DOWN): skip floors (-2 jump) |
| 5 | P4: door open but `mon_served == false` |
| 6 | P6 (UP): claimed UP but floor decreased |
| 7 | P6 (DOWN): claimed DOWN but floor increased |
| 8 | P7: idle correctness (no req + door closed but dir != IDLE) |
| 9 | P5: request not served within `kServe=5` (needs `INJECT_FLOOR`) |
| 10 | P8: request not cleared within `kStarve=15` (needs `INJECT_FLOOR` and longer run) |

### CBMC commands for every injection case

All of these use the same file list and checks.

#### Case 0 (no injection)

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=0 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

#### Case 1 (P1)

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=1 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

#### Case 2 (P2)

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=2 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

#### Case 3 (P3 UP)

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=3 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

#### Case 4 (P3 DOWN)

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=4 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

#### Case 5 (P4)

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=5 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

#### Case 6 (P6 UP)

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=6 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

#### Case 7 (P6 DOWN)

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=7 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

#### Case 8 (P7)

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=8 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

#### Case 9 (P5) — pick a floor

`INJECT_FLOOR` must be `0..4`.

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=9 -DINJECT_FLOOR=0 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main

cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=9 -DINJECT_FLOOR=1 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main

cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=9 -DINJECT_FLOOR=2 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main

cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=9 -DINJECT_FLOOR=3 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main

cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=9 -DINJECT_FLOOR=4 \
	--bounds-check --pointer-check --unwind 16 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

#### Case 10 (P8) — pick a floor and run longer

This case needs more than `kStarve=15` ticks. The example below uses `MAIN_TICKS=20`.

```bash
cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=10 -DINJECT_FLOOR=0 -DMAIN_TICKS=20 \
	--bounds-check --pointer-check --unwind 21 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main

cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=10 -DINJECT_FLOOR=1 -DMAIN_TICKS=20 \
	--bounds-check --pointer-check --unwind 21 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main

cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=10 -DINJECT_FLOOR=2 -DMAIN_TICKS=20 \
	--bounds-check --pointer-check --unwind 21 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main

cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=10 -DINJECT_FLOOR=3 -DMAIN_TICKS=20 \
	--bounds-check --pointer-check --unwind 21 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main

cbmc -DCBMC -DVERIFY -DWITH_COPILOT -DINJECT_CASE=10 -DINJECT_FLOOR=4 -DMAIN_TICKS=20 \
	--bounds-check --pointer-check --unwind 21 \
	system.c elevator.c elevator_mon.c monitor_inputs.c nondet.c \
	--function main
```

## Regenerating the monitor (optional)

If you have Copilot installed for Haskell, you can regenerate `elevator_mon.c` from `ElevatorMon.hs`:

```bash
ghc -O2 ElevatorMon.hs
./ElevatorMon
```

That should emit/overwrite `elevator_mon.c`, `elevator_mon.h`, and `elevator_mon_types.h`.
