CC ?= gcc
CFLAGS ?= -std=c11 -O0 -g -Wall -Wextra -Werror -fno-strict-aliasing

# Reuse the host_single minimal nxtOSEK headers
INCLUDES = -I. -I../../common -I../../../../../host_single/include

# CBMC unwinding needs to cover the longest bounded loops we execute.
UNWIND ?= 80

MON_NAME = steering_sign_mon
MON_C = $(MON_NAME).c
MON_H = $(MON_NAME).h
MON_TYPES = $(MON_NAME)_types.h

SRCS_COMMON = \
	../../../../../nxtgt.c \
	../../common/stubs_cbmc.c \
	trace_steering_sign.c \
	../../common/seu_support.c \
	steering_sign_mon_orig_wrap.c \
	steering_sign_mon_seu_wrap.c \
	harness_crv.c

all: gen

.PHONY: gen

gen: $(MON_C) $(MON_H) $(MON_TYPES)

$(MON_C) $(MON_H) $(MON_TYPES): SteeringSignMon.hs
	runhaskell SteeringSignMon.hs

# CBMC CRV checks (assertion success => NOT CRV; failure => CRV)
.PHONY: cbmc_crv_steering_angle cbmc_crv_bt_right cbmc_crv_motor_steer
.PHONY: crv_summary

cbmc_crv_steering_angle: gen
	cbmc -DCBMC -DRV_MONITOR -DSEU_TARGET=1 $(INCLUDES) \
		--function main \
		--unwind $(UNWIND) \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		--slice-formula \
		--property main.assertion.1 \
		$(SRCS_COMMON)

cbmc_crv_bt_right: gen
	cbmc -DCBMC -DRV_MONITOR -DSEU_TARGET=2 $(INCLUDES) \
		--function main \
		--unwind $(UNWIND) \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		--slice-formula \
		--property main.assertion.1 \
		$(SRCS_COMMON)

cbmc_crv_motor_steer: gen
	cbmc -DCBMC -DRV_MONITOR -DSEU_TARGET=3 $(INCLUDES) \
		--function main \
		--unwind $(UNWIND) \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		--slice-formula \
		--property main.assertion.1 \
		$(SRCS_COMMON)

# Runs all CRV checks and prints an end-of-run summary.
crv_summary: gen
	@chmod +x ./run_crv_summary.sh
	@UNWIND=$(UNWIND) ./run_crv_summary.sh

.PHONY: clean
clean:
	rm -f *.o $(MON_C) $(MON_H) $(MON_TYPES)
