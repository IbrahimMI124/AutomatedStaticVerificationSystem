CC ?= gcc
CFLAGS ?= -std=c11 -O0 -g -Wall -Wextra -Werror -fno-strict-aliasing

# Reuse the host_single minimal nxtOSEK headers
INCLUDES = -I. -I../../common -I../../../../../host_single/include

# CBMC unwinding needs to cover the longest bounded loops we execute.
# In particular, the BT read stub clears a 32-byte buffer.
UNWIND ?= 80

MON_NAME = touch_edc_mon
MON_C = $(MON_NAME).c
MON_H = $(MON_NAME).h
MON_TYPES = $(MON_NAME)_types.h

SRCS_COMMON = \
	../../../../../nxtgt.c \
	../../common/stubs_cbmc.c \
	trace_touch_edc.c \
	../../common/seu_support.c \
	touch_edc_mon_orig_wrap.c \
	touch_edc_mon_seu_wrap.c \
	harness_crv.c

SRCS_SANITY_EDCFLIP = \
	../../../../../nxtgt.c \
	../../common/stubs_cbmc.c \
	trace_touch_edc.c \
	../../common/seu_support.c \
	touch_edc_mon_orig_wrap.c \
	touch_edc_mon_seu_wrap.c \
	harness_sanity_edcflip.c

all: gen

.PHONY: gen

gen: $(MON_C) $(MON_H) $(MON_TYPES)

$(MON_C) $(MON_H) $(MON_TYPES): TouchEdcMon.hs
	runhaskell TouchEdcMon.hs

# CBMC CRV checks (assertion success => NOT CRV; failure => CRV)
.PHONY: cbmc_crv_touch cbmc_crv_edc cbmc_crv_touch_state cbmc_sanity_edc_flip
.PHONY: crv_summary
.PHONY: cbmc_crv_edc_trace cbmc_crv_touch_state_trace
.PHONY: crv_traces

cbmc_crv_touch: gen
	cbmc -DCBMC -DRV_MONITOR -DSEU_TARGET=1 $(INCLUDES) \
		--function main \
		--unwind $(UNWIND) \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		--slice-formula \
		--property main.assertion.1 \
		$(SRCS_COMMON)

cbmc_crv_edc: gen
	cbmc -DCBMC -DRV_MONITOR -DSEU_TARGET=2 $(INCLUDES) \
		--function main \
		--unwind $(UNWIND) \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		--slice-formula \
		--property main.assertion.1 \
		$(SRCS_COMMON)

cbmc_crv_steering: gen
	cbmc -DCBMC -DRV_MONITOR -DSEU_TARGET=4 $(INCLUDES) \
		--function main \
		--unwind $(UNWIND) \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		--slice-formula \
		--property main.assertion.1 \
		$(SRCS_COMMON)

cbmc_crv_touch_state: gen
	cbmc -DCBMC -DRV_MONITOR -DSEU_TARGET=3 $(INCLUDES) \
		--function main \
		--unwind $(UNWIND) \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		--slice-formula \
		--property main.assertion.1 \
		$(SRCS_COMMON)

# Trace versions (helpful for CRV failures): keep the same semantics, but show the counterexample trace.
cbmc_crv_edc_trace: gen
	@bash -lc 'set +e; cbmc -DCBMC -DRV_MONITOR -DSEU_TARGET=2 $(INCLUDES) \
		--function main \
		--unwind $(UNWIND) \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		--trace \
		--property main.assertion.1 \
		$(SRCS_COMMON); rc=$$?; if [ $$rc -eq 10 ]; then exit 0; fi; exit $$rc'

cbmc_crv_touch_state_trace: gen
	@bash -lc 'set +e; cbmc -DCBMC -DRV_MONITOR -DSEU_TARGET=3 $(INCLUDES) \
		--function main \
		--unwind $(UNWIND) \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		--trace \
		--property main.assertion.1 \
		$(SRCS_COMMON); rc=$$?; if [ $$rc -eq 10 ]; then exit 0; fi; exit $$rc'

crv_traces: gen
	@$(MAKE) -s cbmc_crv_edc_trace
	@$(MAKE) -s cbmc_crv_touch_state_trace

cbmc_sanity_edc_flip: gen
	cbmc -DCBMC -DRV_MONITOR $(INCLUDES) \
		--function main \
		--unwind $(UNWIND) \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		$(SRCS_SANITY_EDCFLIP)

# Runs all CRV checks and prints an end-of-run summary (continues on expected CBMC failures).
crv_summary: gen
	@chmod +x ./run_crv_summary.sh
	@UNWIND=$(UNWIND) ./run_crv_summary.sh

.PHONY: clean
clean:
	rm -f *.o $(MON_C) $(MON_H) $(MON_TYPES)
