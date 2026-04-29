# CBMC + Copilot CRV Verification Framework

This directory contains a formal verification framework that identifies **Conditionally Relevant Variables (CRV)** in the NXT GT car controller (`nxtgt.c`). It combines two tools:

- **Copilot** (Haskell DSL) — generates a C runtime monitor from a temporal-logic safety property.
- **CBMC** (C Bounded Model Checker) — exhaustively searches for single-bit upsets that can violate the property.

---

## Table of Contents

1. [What is a CRV?](#1-what-is-a-crv)
2. [Safety Property P7](#2-safety-property-p7)
3. [File-by-File Breakdown](#3-file-by-file-breakdown)
4. [How the Verification Works](#4-how-the-verification-works)
5. [How nxtgt.c Works Without Real Sensors](#5-how-nxtgtc-works-without-real-sensors)
6. [Input Generation Strategy](#6-input-generation-strategy)
7. [Makefile Targets](#7-makefile-targets)
8. [Results](#8-results)
9. [How to Add a New Variable](#9-how-to-add-a-new-variable)

---

## 1. What is a CRV?

A variable is **Conditionally Relevant (CRV)** to a safety property if a single-bit flip (Single Event Upset / SEU) in that variable can cause the system to violate the property **when it otherwise would not have**.

| Outcome | Meaning |
| :--- | :--- |
| **NOT CRV** | No single-bit flip in this variable can, by itself, cause a new safety violation. The system is naturally resilient. |
| **CRV** | There exists at least one scenario where a single-bit flip causes a violation. This variable may need hardware protection (e.g., ECC, TMR). |

---

## 2. Safety Property P7

Property P7 governs the coupling between the **touch sensor** and the **Electronic Differential Control (EDC)** flag:

> **P7**: *The EDC flag must toggle if and only if the touch sensor has a rising edge.*

This decomposes into two sub-properties:

| Sub-property | Temporal Logic | Plain English |
| :--- | :--- | :--- |
| P7a | G(touch_rises → EDC_toggles) | If the touch sensor goes from 0 to 1, EDC must change. |
| P7b | G(touch_steady → ¬EDC_toggles) | If the touch sensor stays the same, EDC must not change. |

The first time step is always skipped (no previous values to compare against).

---

## 3. File-by-File Breakdown

### 3.1 `TouchEdcMon.hs` — The Copilot Specification

**Purpose**: Defines Property P7 as a Copilot stream specification and generates C monitor code.

**Key contents**:
```haskell
-- Two external streams (published by the C glue code before each step)
fr_touch    :: Stream Bool     -- Is touch pressed this step?
fr_edc_flag :: Stream Int8     -- Current EDC flag value

-- History tracking using Copilot's delay operator
let prevTouch = [False] ++ fr_touch    -- touch value from previous step
let prevEdc   = [0]     ++ fr_edc_flag -- EDC value from previous step

-- Derived boolean conditions
let touchRises  = fr_touch && not prevTouch     -- rising edge detected
let touchSteady = fr_touch == prevTouch         -- no edge
let edcToggles  = fr_edc_flag /= prevEdc        -- EDC changed since last step

-- Property P7
let p7_body = (not touchRises || edcToggles) && (not touchSteady || not edcToggles)
let p7 = first || p7_body   -- skip the initial step

-- When P7 is violated, call the C function touch_edc_violation()
trigger "touch_edc_violation" (not p7) []
```

**How to regenerate**: `runhaskell TouchEdcMon.hs` (or `make gen`)

**Output files**: `touch_edc_mon.c`, `touch_edc_mon.h`, `touch_edc_mon_types.h`

---

### 3.2 `touch_edc_mon.c` / `touch_edc_mon.h` / `touch_edc_mon_types.h` — Generated Monitor

**Purpose**: Auto-generated C code that implements the Copilot streams as circular buffers and evaluates the P7 guard on every call to `step()`.

**Key internals of `touch_edc_mon.c`**:

| Element | Role |
| :--- | :--- |
| `fr_touch_cpy`, `fr_edc_flag_cpy` | Local copies of the external streams, snapshotted at the start of each `step()`. |
| `s0[1]` (init: `true`) | The "first step" flag. Starts `true`, becomes `false` after the first step. Used to skip P7 checking on step 0. |
| `s1[1]` (init: `false`) | Previous touch value (`prevTouch`). Updated to `fr_touch_cpy` at each step. |
| `s2[1]` (init: `0`) | Previous EDC flag value (`prevEdc`). Updated to `fr_edc_flag_cpy` at each step. |
| `touch_edc_violation_0_guard()` | The negation of P7. Returns `true` when the property is violated. |
| `step()` | Main entry point. Snapshots inputs → evaluates guard → calls `touch_edc_violation()` if violated → advances stream history. |

**`touch_edc_mon.h`** exports:
```c
extern bool fr_touch;           // Must be set before calling step()
extern int8_t fr_edc_flag;      // Must be set before calling step()
void touch_edc_violation(void); // Called by the monitor when P7 is violated
void step(void);                // Advance the monitor by one time step
```

### 3.3 `touch_edc_mon_orig_wrap.c` and `touch_edc_mon_seu_wrap.c` — Monitor Wrappers

**Purpose**: We need **two independent instances** of the same monitor — one for the original run and one for the SEU run. Since `touch_edc_mon.c` uses `static` variables, we cannot simply link it twice (symbol collisions). These wrappers solve this using preprocessor renaming:

**`touch_edc_mon_orig_wrap.c`**:
```c
#define step touch_edc_step_orig
#define touch_edc_violation touch_edc_violation_orig
#include "touch_edc_mon.c"
```

**`touch_edc_mon_seu_wrap.c`**:
```c
#define step touch_edc_step_seu
#define touch_edc_violation touch_edc_violation_seu
#include "touch_edc_mon.c"
```

Each wrapper `#include`s the `.c` file directly (not the `.h`), which causes the compiler to create a completely separate copy of all static variables. The result is two monitors with independent state that can be stepped and queried separately.

---

### 3.4 `trace_touch_edc.c` — Monitor Glue Layer

**Purpose**: Bridges the gap between the harness and the Copilot monitor. Provides the API that `harness_crv.c` calls.

**What it defines**:

| Function / Variable | Role |
| :--- | :--- |
| `bool fr_touch` | Global extern read by the generated monitor. Set by `p7_publish()`. |
| `int8_t fr_edc_flag` | Global extern read by the generated monitor. Set by `p7_publish()`. |
| `p7_publish(touch_pressed, edc_flag)` | Writes the current controller outputs into the monitor's input globals. Called by the harness after each `TaskControl()` invocation. |
| `p7_violation_orig` / `p7_violation_seu` | **Latching flags**. Once set to `true`, they stay `true` for the remainder of the run. |
| `touch_edc_violation_orig()` | Called by the "orig" monitor instance when P7 is violated. Sets `p7_violation_orig = true`. |
| `touch_edc_violation_seu()` | Called by the "SEU" monitor instance when P7 is violated. Sets `p7_violation_seu = true`. |
| `p7_monitor_reset_orig()` / `p7_monitor_reset_seu()` | Clears the violation latch before starting a new run. |
| `p7_monitor_violation_orig()` / `p7_monitor_violation_seu()` | Returns whether a violation was detected during the run. |

---

### 3.5 `harness_crv.c` — The CRV Verification Harness (Main Entry Point)

**Purpose**: The `main()` function that CBMC analyzes. Orchestrates the dual-execution comparison.

**Key sections**:

#### SEU Target Selection (compile-time)
```c
#define SEU_TOUCH_INPUT    1
#define SEU_EDC_FLAG       2
#define SEU_TOUCH_STATE    3
#define SEU_STEERING_ANGLE 4

// Selected via -DSEU_TARGET=N on the CBMC command line
```

#### Bit-flip Helpers
```c
static uint8_t flip_u8_bit(uint8_t v, uint8_t bit);  // XOR a single bit in a uint8
static int8_t  flip_s8_bit(int8_t v, uint8_t bit);   // XOR a single bit in an int8
```

#### `run_original(touch_seq)` — The Baseline Run
Executes the controller for `STEPS` iterations (default 6) with no fault injection:
```
For each step t:
  1. Set rv_touch_input = touch_seq[t]
  2. Call TaskControl()                  ← runs the real controller code
  3. Publish (touch_pressed, EDC_flag) to the "orig" monitor
  4. Step the "orig" monitor
```

#### `run_seu(touch_seq)` — The Fault-Injected Run
Same as `run_original`, but with a single-bit flip applied to the target variable at one nondeterministically chosen step:
```
seu_step = nondet_u8()   (constrained to [1, STEPS))
seu_bit  = nondet_u8()   (constrained to [0, 8))

For each step t:
  1. Set rv_touch_input = touch_seq[t]  (or flipped, if SEU_TARGET == 1 and t == seu_step)
  2. If SEU_TARGET matches and t == seu_step: flip the target variable
  3. Call TaskControl()
  4. Publish to the "SEU" monitor
  5. Step the "SEU" monitor
```

#### `main()` — The Orchestrator
```
1. Generate symbolic touch input sequence: touch_seq[0..STEPS-1] ∈ {0, 1}
2. Initialize controller state (EDC_flag = EDC_OFF, touch_state = 0)
3. Run original execution → record violation_original
4. Re-initialize controller to same state
5. Run SEU execution (same inputs, one bit-flip) → record violation_seu
6. Assert: violation_original == violation_seu
```

The assertion `violation_original == violation_seu` is the **CRV check**:
- If CBMC proves it holds → **NOT CRV**
- If CBMC finds a counterexample → **CRV**

---

### 3.6 `stubs_cbmc.c` — Deterministic Hardware Stubs

**Purpose**: Replaces all nxtOSEK hardware API functions with minimal stubs so that CBMC can analyze the controller without real hardware.

| Stubbed Function | Behavior | Why |
| :--- | :--- | :--- |
| `ecrobot_get_touch_sensor()` | Returns `rv_touch_input ? 1 : 0` | **The only non-trivial stub.** Bridges the harness's symbolic input to the controller's sensor read. |
| `ecrobot_read_bt_packet()` | Fills buffer with zeros | Ensures BT commands are deterministic (speed=0, steer=0). |
| `nxt_motor_get_count()` | Returns 0 | No motor feedback simulation needed for P7. |
| `nxt_motor_set_speed()` | No-op | Motor outputs are irrelevant to P7. |
| `ecrobot_get_sonar_sensor()` | Returns 0 | Sonar is irrelevant to P7. |
| `SignalCounter()` | Returns `E_OK` | OSEK counter stub. |
| `ShutdownOS()` | No-op | Prevents abort during verification. |

---

### 3.7 `seu_support.c` / `seu_support.h` — SEU Injection Helpers

**Purpose**: Provides utility functions for nondeterministic single-bit flips. These are used by the **sanity harness** (`harness_sanity_edcflip.c`) but not by the main CRV harness (which does its own inline flipping).

| Function | Behavior |
| :--- | :--- |
| `seu_reset()` | Resets the "already flipped" flag. Ensures at most one flip per run. |
| `seu_maybe_flip_u8(v)` | Under CBMC: nondeterministically decides whether to flip a random bit. At most once per run. |
| `seu_maybe_flip_s8(v)` | Same, for signed 8-bit values. |

---

### 3.8 `harness_sanity_edcflip.c` — Sanity Check Harness

**Purpose**: A **deterministic** test to validate that the monitor and harness infrastructure are wired correctly before trusting the nondeterministic CRV results.

**What it does**:
1. Hardcodes a steady touch input sequence: `{0, 0, 0, 0}` (touch never pressed).
2. **Original run**: Asserts P7 is **not** violated (no rising edge → EDC never toggles → safe).
3. **SEU run**: Forces bit-0 of `EDC_flag` to flip at step 1 → EDC changes without a touch event → asserts P7 **is** violated.

If both assertions pass, we know the monitor correctly detects violations and the plumbing is correct.

---

### 3.9 `Makefile` — Build and Verification Targets

**Purpose**: Orchestrates monitor generation and CBMC invocations.

| Target | What it does |
| :--- | :--- |
| `gen` | Runs `runhaskell TouchEdcMon.hs` to generate `touch_edc_mon.c/h`. |
| `cbmc_crv_touch` | CBMC check with `SEU_TARGET=1` (touch input). |
| `cbmc_crv_edc` | CBMC check with `SEU_TARGET=2` (EDC flag). |
| `cbmc_crv_touch_state` | CBMC check with `SEU_TARGET=3` (edge tracker). |
| `cbmc_crv_steering` | CBMC check with `SEU_TARGET=4` (steering angle). |
| `cbmc_crv_edc_trace` | Same as `cbmc_crv_edc` but with `--trace` for counterexample output. |
| `cbmc_crv_touch_state_trace` | Same as `cbmc_crv_touch_state` but with `--trace`. |
| `crv_summary` | Runs all CRV checks via `run_crv_summary.sh` and prints a summary table. |
| `cbmc_sanity_edc_flip` | Runs the deterministic sanity check harness. |
| `clean` | Removes generated files. |

**Key CBMC flags used**:

| Flag | Purpose |
| :--- | :--- |
| `-DCBMC` | Enables CBMC-specific code paths (`nondet`, `__CPROVER_assume/assert`). |
| `-DRV_MONITOR` | Enables the `rv_get/set` accessor functions in `nxtgt.c`. |
| `-DSEU_TARGET=N` | Selects which variable to fault-inject. |
| `--unwind N` | Sets loop unwinding depth (default 80). |
| `--property main.assertion.1` | Only checks the CRV assertion, ignoring all other properties. |
| `--slice-formula` | Removes irrelevant variables from the SAT formula for performance. |
| `--no-standard-checks` etc. | Disables built-in checks (bounds, pointers, overflow) to focus solely on the CRV assertion. |

---

### 3.10 `run_crv_summary.sh` — Summary Script

**Purpose**: Runs all CRV checks sequentially and prints a human-readable summary table.

**How it works**:
1. Invokes each Makefile target via `make -s`.
2. Captures CBMC's stdout/stderr output.
3. Greps for `[main.assertion.1] ... : FAILURE` or `... : SUCCESS`.
4. Prints a formatted line: `variable_name : CRV` or `variable_name : NOT CRV`.
5. Continues on failures (expected for CRV variables).

---

## 4. How the Verification Works

The framework uses a **Dual-Execution Comparison** strategy:

```
┌──────────────────────────────────────────────────────────┐
│  main()                                                  │
│                                                          │
│  1. Generate symbolic touch_seq[0..5] ∈ {0, 1}           │
│                                                          │
│  2. ┌─── Original Run ───┐   ┌──── SEU Run ────┐         │
│     │ Same init state     │   │ Same init state  │       |
│     │ Same touch_seq      │   │ Same touch_seq   │       │
│     │ No fault            │   │ 1 bit-flip at    │       │
│     │                     │   │ nondet step/bit  │       │
│     │ → violation_orig    │   │ → violation_seu  │       │
│     └─────────────────────┘   └──────────────────┘       │
│                                                          │
│  3. Assert: violation_orig == violation_seu              │
│     SUCCESS → NOT CRV    |    FAILURE → CRV              │
└──────────────────────────────────────────────────────────┘
```

**Why this works**: If a bit-flip can cause a *new* violation (one that doesn't exist in the original run), then the variable's integrity is critical to safety. CBMC explores **all** possible input sequences and **all** possible fault injection points exhaustively.

---

## 5. How `nxtgt.c` Works Without Real Sensors

In normal operation on a Lego NXT robot, `nxtgt.c` calls hardware API functions to interact with physical sensors and motors:

| API Call in `nxtgt.c` | What it does on real hardware |
| :--- | :--- |
| `ecrobot_get_touch_sensor(NXT_PORT_S4)` | Reads the physical touch sensor (pressed = 1, released = 0) |
| `nxt_motor_get_count(MOTOR_STEERING)` | Reads the steering motor encoder position (degrees) |
| `ecrobot_read_bt_packet(buf, 32)` | Reads speed/steering commands from the Bluetooth gamepad |
| `nxt_motor_set_speed(port, speed, brake)` | Drives the wheel/steering motors |
| `ecrobot_get_sonar_sensor(NXT_PORT_S2)` | Reads the ultrasonic distance sensor |

None of these work without the NXT hardware and nxtOSEK firmware. The CBMC verification framework removes the hardware dependency using a **three-layer substitution architecture**:

### Layer 1: Minimal Headers (`host_single/include/`)

Instead of including the real nxtOSEK headers (which pull in ARM-specific code, RTOS internals, etc.), the Makefile passes `-I../host_single/include`. These provide:

- **Type aliases only**: `S8 → int8_t`, `U8 → uint8_t`, `S32 → int32_t`, plus port enums (`NXT_PORT_A`, `NXT_PORT_S4`, etc.)
- **OSEK macros redefined as plain C**: `TASK(name)` → `void name(void)`, `TerminateTask()` → `return`

This means `nxtgt.c` compiles on a standard host machine (x86/x64) without any NXT cross-compilation toolchain.

### Layer 2: Stub Functions (`stubs_cbmc.c`)

Every hardware API is replaced with a **deterministic stub**:

```c
// The harness controls this variable
uint8_t rv_touch_input;

// Touch sensor returns whatever the harness sets
U8 ecrobot_get_touch_sensor(int port) {
    return (U8)(rv_touch_input ? 1u : 0u);   // ← harness-driven
}

// Motor encoder always returns 0 (no physical steering)
S32 nxt_motor_get_count(int port) {
    return 0;                                  // ← deterministic
}

// Bluetooth always returns zeros (no gamepad input)
void ecrobot_read_bt_packet(U8 *buf, size_t len) {
    for (size_t i = 0; i < len; i++) buf[i] = 0;  // ← deterministic
}

// Motor writes are simply discarded
void nxt_motor_set_speed(int port, S32 speed, int brake) {
    (void)port; (void)speed; (void)brake;      // ← no-op
}
```

**Key insight**: The only sensor that *matters* for the property being verified (touch→EDC toggle) is the **touch sensor**. So it is the only one made controllable via `rv_touch_input`. Everything else is zeroed out or no-oped because it is irrelevant to P7.

### Layer 3: The Harness (`harness_crv.c`)

The harness feeds symbolic values into `rv_touch_input` before each `TaskControl()` call:

```c
// CBMC generates ALL possible 6-step touch sequences
uint8_t touch_seq[STEPS];
for (unsigned t = 0; t < STEPS; t++) {
    touch_seq[t] = nondet_u8();                // symbolic value
    __CPROVER_assume(touch_seq[t] <= 1u);      // constrain to {0, 1}
}

// Each step: set the sensor value, then run the controller
for (unsigned t = 0; t < STEPS; t++) {
    rv_touch_input = touch_seq[t];             // "sensor reading"
    TaskControl();                              // real nxtgt.c logic
}
```

`nondet_u8()` is a **CBMC built-in** that represents a completely unconstrained value. The `__CPROVER_assume` constraint then restricts it to valid touch sensor readings (0 or 1). CBMC's SAT solver exhaustively explores *every* possible combination — this is not random testing, it is **complete formal verification** over the bounded input space.

### Complete Data Flow

```
CBMC nondet ──► touch_seq[t] ──► rv_touch_input ──► ecrobot_get_touch_sensor() stub
                                                           │
                                                           ▼
                                                    nxtgt.c TaskControl()
                                                           │
                                                    reads touch_sensor
                                                    updates EDC_flag
                                                    updates touch_sensor_state
                                                           │
                                                           ▼
                                                    rv_get_EDC_flag() ──► P7 monitor
```

In the SEU run, one of the intermediate variables (`rv_touch_input`, `EDC_flag`, `touch_sensor_state`, or `steering_angle` — depending on `SEU_TARGET`) gets a single bit flipped at a nondeterministically chosen step, and CBMC checks whether that corruption can cause the P7 safety property to diverge from the original run.

### Sensor Simulation Summary

| Sensor | Simulation Method | Rationale |
| :--- | :--- | :--- |
| **Touch sensor** | `nondet_u8()` constrained to `{0, 1}` | This is the input relevant to P7. CBMC explores all possible 6-step press/release sequences. |
| **Motor encoder** | Always returns `0` | Steering angle is an internal state variable, not an external input. Fixed to simplify the model. |
| **Bluetooth gamepad** | Always returns `{0, 0, ...}` | Speed/steering commands are irrelevant to the touch→EDC property. |
| **Sonar** | Always returns `0` | Only used for data logging in `TaskSonar`, has no control effect on `TaskControl`. |

---

## 6. Input Generation Strategy

| Input | Source | Method |
| :--- | :--- | :--- |
| Touch sensor | `harness_crv.c` | **Symbolic** (`nondet_u8()`, constrained to `{0, 1}`). CBMC explores all possible sequences. |
| Bluetooth (speed/steer) | `stubs_cbmc.c` | **Deterministic** (always zero). Irrelevant to P7. |
| Motor feedback | `stubs_cbmc.c` | **Deterministic** (always zero). Irrelevant to P7. |
| Sonar | `stubs_cbmc.c` | **Deterministic** (always zero). Irrelevant to P7. |
| SEU step | `harness_crv.c` | **Symbolic** (`nondet_u8()`, constrained to `[1, STEPS)`). |
| SEU bit position | `harness_crv.c` | **Symbolic** (`nondet_u8()`, constrained to `[0, 8)`). |

Only the touch sensor is made symbolic because P7 only concerns touch/EDC coupling. Making other inputs symbolic would explode the state space without improving coverage of P7.

---

## 7. Makefile Targets

### Quick Reference

```bash
# Generate the Copilot monitor (prerequisite for all checks)
make gen

# Run all CRV checks with summary output
make crv_summary

# Run individual variable checks
make cbmc_crv_touch          # SEU_TARGET=1: touch input
make cbmc_crv_edc            # SEU_TARGET=2: EDC_flag
make cbmc_crv_touch_state    # SEU_TARGET=3: touch_sensor_state
make cbmc_crv_steering       # SEU_TARGET=4: steering_angle

# Get counterexample traces for CRV variables
make cbmc_crv_edc_trace
make cbmc_crv_touch_state_trace

# Sanity check (validates monitor wiring)
make cbmc_sanity_edc_flip

# Override loop unwinding depth
UNWIND=120 make crv_summary

# Clean generated files
make clean
```

---

## 8. Results

| Variable | SEU_TARGET | Result | Explanation |
| :--- | :--- | :--- | :--- |
| `touch_input` | 1 | **NOT CRV** | The controller and monitor both see the same (possibly flipped) input value, so they stay synchronized. The property holds relative to the perceived input. |
| `EDC_flag` | 2 | **CRV** | A bit-flip directly corrupts the EDC state, causing it to change without any touch event. The monitor detects this as a violation. |
| `touch_sensor_state` | 3 | **CRV** | Corrupting the edge-tracker can create a "ghost" rising edge (where none occurred) or mask a real one. Either case violates P7. |
| `steering_angle` | 4 | **NOT CRV** | The steering angle is used downstream in the motor control loop, which is completely independent of the touch/EDC toggle logic. A bit-flip here cannot affect P7. |

---

## 9. How to Add a New Variable

To check a new variable for CRV:

1. **Expose it** in `nxtgt.c`: Add `rv_get_X()` / `rv_set_X()` accessors inside `#ifdef RV_MONITOR`.
2. **Register it** in `harness_crv.c`: Define a new `SEU_*` macro ID and add a `#if SEU_TARGET == ...` block in `run_seu()`.
3. **Add a Makefile target**: Copy an existing `cbmc_crv_*` target, changing `-DSEU_TARGET=N`.
4. **Update the summary script**: Add a `run_one` line in `run_crv_summary.sh`.

Total effort: ~15 lines of code across 4 files.
