# CBMC + Copilot CRV check: P7 Touch toggles EDC exactly on rising edge

Property P7 (as a runtime monitor):

- $G(\text{touch rises} \rightarrow \text{EDC toggles})$
- $G(\text{touch steady} \rightarrow \text{EDC unchanged})$

This folder builds a Copilot monitor and a CBMC harness that compares:

1) an **original** execution (no upset)
2) a **SEU** execution (same init + same touch inputs, but with a single-bit upset injected into one selected variable)

It then asserts `violation_original == violation_seu`.

- **CBMC success** => the variable is **not** conditionally relevant (not CRV)
- **Assertion failure** => the variable **is** conditionally relevant (CRV)

## Run

From this directory:

```bash
make crv_summary

make cbmc_crv_touch
make cbmc_crv_edc
make cbmc_crv_touch_state

# Optional: sanity check that a forced EDC_flag flip is detectable
make cbmc_sanity_edc_flip
```

### Expected outcomes (with default settings)

- `cbmc_crv_touch`: **SUCCESS** (touch input is **not** CRV)
- `cbmc_crv_edc`: **FAILURE** (EDC flag is **CRV**)
- `cbmc_crv_touch_state`: **FAILURE** (touch edge-tracker state is **CRV**)

### Getting a counterexample trace

```bash
make cbmc_crv_edc_trace
make cbmc_crv_touch_state_trace
```

Note: these trace targets may print `VERIFICATION FAILED` (that is the expected outcome for CRV variables), but the `make` target will still exit successfully so you can run multiple traces in sequence.

## Notes

- These CBMC checks rely on sufficient loop unwinding (see `UNWIND` in `Makefile`).
- The SEU is modeled as exactly one single-bit flip at a nondet-selected step after the first monitor sample.

- Only minimal, monitor-facing accessors were added to `../nxtgt.c` under `-DRV_MONITOR`.
- The harness uses deterministic stubs for non-touch inputs so both runs share identical inputs.
