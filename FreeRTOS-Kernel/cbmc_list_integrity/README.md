# CBMC + Copilot: FreeRTOS list integrity (single safety property)

This folder proves one safety property about `list.c` using a Copilot-generated runtime monitor, without editing FreeRTOS kernel source files.

## Property

- **Safety:** after every list operation, the list end marker and local circular links are consistent.
- Implemented as a single boolean `fr_list_ok` published from trace hooks; the Copilot monitor triggers `list_integrity()` if it ever becomes false.

## Files

- `ListIntegrityMon.hs`: Copilot spec (`trigger "list_integrity" (not fr_list_ok) []`).
- `trace_list_integrity.c`: computes `fr_list_ok`, calls `step()` after each list API call, and latches `fr_list_violation` when triggered.
- `FreeRTOSConfig.h`: minimal config + defines `traceENTER_*`/`traceRETURN_*` macros for the list APIs.
- `portmacro.h`: minimal CBMC-friendly port stubs (only for compiling `list.c`).
- `harness.c`: bounded nondet sequence of insert/remove operations + assertion that the trigger never fires.

## Run

```bash
cd FreeRTOS-Kernel/cbmc_list_integrity
make clean all
make run
```

## Prove with CBMC

```bash
cd FreeRTOS-Kernel/cbmc_list_integrity
make cbmc
```

## SEU (single-event upset) relevance check

To test whether *one specific variable* is **conditionally relevant** to the safety property under a modeled SEU (a single bit-flip, injected at most once at a nondeterministic time), run:

```bash
cd FreeRTOS-Kernel/cbmc_list_integrity
make cbmc_seu_itemvalue
```

Interpretation:
- **VERIFICATION FAILED / SAT**: there exists a time + bit-flip of `pxList->xListEnd.xItemValue` that makes the runtime monitor fire (variable is relevant).
- **VERIFICATION SUCCESSFUL / UNSAT**: no such single bit-flip (as modeled) can violate the monitor property within the current bounds.

If you want this to cover more structure (e.g., sortedness, `uxNumberOfItems` matches reachable nodes, or `pxIndex` invariants), we can extend `fr_list_check_basic()` and keep the Copilot side unchanged (still a single `fr_list_ok`).
