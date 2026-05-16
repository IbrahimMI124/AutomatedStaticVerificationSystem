#!/usr/bin/env bash
set -u

cd "$(dirname "$0")"

UNWIND_VAL="${UNWIND:-80}"

run_one() {
  local label="$1"
  local make_target="$2"

  local out
  out="$({ UNWIND="$UNWIND_VAL" make -s "$make_target"; } 2>&1)"
  local rc=$?

  if printf '%s\n' "$out" | grep -q "\[main\.assertion\.1\].*: FAILURE"; then
    printf '%-22s : CRV (assertion FAILED)\n' "$label"
    return 10
  fi
  if printf '%s\n' "$out" | grep -q "\[main\.assertion\.1\].*: SUCCESS"; then
    printf '%-22s : NOT CRV (assertion SUCCESS)\n' "$label"
    return 0
  fi

  printf '%-22s : UNKNOWN (could not find main.assertion.1 result)\n' "$label"
  printf '%s\n' "$out" | tail -n 40
  return "$rc"
}

echo "UNWIND=${UNWIND_VAL}"
echo

UNWIND="$UNWIND_VAL" make -s gen >/dev/null 2>&1 || true

run_one "EDC_flag"       "cbmc_crv_edc_flag"       || true
run_one "steering_angle" "cbmc_crv_steering_angle" || true
run_one "bt_left"        "cbmc_crv_bt_left"        || true
run_one "motor_left"     "cbmc_crv_motor_left"     || true
