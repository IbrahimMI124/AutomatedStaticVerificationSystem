CC ?= gcc
CFLAGS ?= -std=c11 -O0 -g -Wall -Wextra -Werror -fno-strict-aliasing

INCLUDES = -I. -I../include

MON_NAME = list_integrity_mon
MON_C = $(MON_NAME).c
MON_H = $(MON_NAME).h
MON_TYPES = $(MON_NAME)_types.h

SRCS = \
	../list.c \
	harness.c \
	trace_list_integrity.c \
	$(MON_C)

all: gen build

# Generate Copilot monitor C code
.PHONY: gen

gen: $(MON_C) $(MON_H) $(MON_TYPES)

$(MON_C) $(MON_H) $(MON_TYPES): ListIntegrityMon.hs
	runhaskell ListIntegrityMon.hs

# Build and run with gcc (sanity)
.PHONY: build

build: list_integrity_demo

list_integrity_demo: $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SRCS)

.PHONY: run
run: list_integrity_demo
	./list_integrity_demo

# Prove under CBMC
.PHONY: cbmc
cbmc: gen
	cbmc -DCBMC $(INCLUDES) \
		--function main \
		--unwind 10 \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		--slice-formula \
		--property main.assertion.1 \
		../list.c harness.c trace_list_integrity.c $(MON_C)

# Check conditional relevance of a single variable under a one-bit SEU.
# If CBMC reports SAT/FAIL, that variable upset can violate the monitor property.
.PHONY: cbmc_seu_itemvalue
cbmc_seu_itemvalue: gen
	cbmc -DCBMC -DSEU_XLISTEND_ITEMVALUE $(INCLUDES) \
		--function main \
		--unwind 10 \
		--no-standard-checks --no-built-in-assertions \
		--no-bounds-check --no-pointer-check --no-signed-overflow-check --no-div-by-zero-check \
		--no-undefined-shift-check --no-pointer-primitive-check \
		--slice-formula \
		--property main.assertion.1 \
		--trace \
		../list.c harness.c trace_list_integrity.c $(MON_C)

.PHONY: clean
clean:
	rm -f list_integrity_demo *.o $(MON_C) $(MON_H) $(MON_TYPES)
