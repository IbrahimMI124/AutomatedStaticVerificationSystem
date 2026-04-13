# Runtime Monitor Demo Systems (PRNG/Nondet)

This folder contains multiple small “systems” that pair:

- a Copilot specification (`*.hs`) compiled to a C runtime monitor (`step()`), and
- a C harness (`main.c`) that simulates a system using deterministic PRNG values (or nondeterministic values under CBMC).

Each subfolder is buildable independently via `make`.

## Quick start

Build everything:

```bash
make -C rv_systems
```

Run one system (example):

```bash
make -C rv_systems/automotive_system run
```

## Time base

Harnesses define `TICK_MS` (default 100ms). “Within N cycles” uses N steps.

## Fault injection

Some harnesses support `INJECT_CASE` to force a violation scenario.

Example:

```bash
make -C rv_systems/automotive_system clean all CFLAGS="-DINJECT_CASE=1"
./rv_systems/automotive_system/automotive_system
```
