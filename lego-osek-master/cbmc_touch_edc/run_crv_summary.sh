#!/usr/bin/env bash
set -u

# Runs all CRV checks and prints a final CRV/NOT-CRV summary.
# Convention used in this repo:
# - CBMC property (main.assertion.1) SUCCESS  => NOT CRV
# - CBMC property (main.assertion.1) FAILURE  => CRV

cd "$(dirname "$0")"

UNWIND_VAL="${UNWIND:-80}"

run_one() {
  local label="$1"
  local make_target="$2"

  # Run and capture output; do not stop on non-zero (CRV shows up as Error 10).
  local out
  out="$({ UNWIND="$UNWIND_VAL" make -s "$make_target"; } 2>&1)"
  local rc=$?

  # Decide based on the CRV assertion result line.
  # We expect only one property here: main.assertion.1.
  if printf '%s\n' "$out" | grep -q "\[main\.assertion\.1\].*: FAILURE"; then
    printf '%-18s : CRV (assertion FAILED)\n' "$label"
    return 10
  fi
  if printf '%s\n' "$out" | grep -q "\[main\.assertion\.1\].*: SUCCESS"; then
    printf '%-18s : NOT CRV (assertion SUCCESS)\n' "$label"
    return 0
  fi

  printf '%-18s : UNKNOWN (could not find main.assertion.1 result)\n' "$label"
  printf '%s\n' "$out" | tail -n 40
  return "$rc"
}

echo "UNWIND=${UNWIND_VAL}"
echo

# Ensure the monitor is generated first (quietly).
UNWIND="$UNWIND_VAL" make -s gen >/dev/null 2>&1 || true

run_one "touch input"       "cbmc_crv_touch"       || true
run_one "EDC_flag"          "cbmc_crv_edc"         || true
run_one "touch_sensor_state" "cbmc_crv_touch_state" || true

echo
printf '%s\n' "Tip: run with traces (for CRV cases):" \
  "  make cbmc_crv_edc_trace" \
  "  make cbmc_crv_touch_state_trace" \
  "" \
  "Or override unwind: UNWIND=120 make crv_summary"
