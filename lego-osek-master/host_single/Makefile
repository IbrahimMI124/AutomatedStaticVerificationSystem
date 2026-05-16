CC ?= gcc
CFLAGS ?= -std=c11 -O0 -g -Wall -Wextra -Werror \
	-Wno-unused-but-set-variable -Wno-unused-variable

INCLUDES = -I./include -I..

SRCS_NXTGT = \
	../nxtgt.c \
	stubs.c \
	osek_sim.c \
	main.c

SRCS_RMS = \
	../rms.c \
	stubs.c \
	osek_sim.c \
	main.c

all: host_single_demo host_single_rms_demo

host_single_demo: $(SRCS_NXTGT)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SRCS_NXTGT)

host_single_rms_demo: $(SRCS_RMS)
	$(CC) $(CFLAGS) $(INCLUDES) -DRMS_APP -o $@ $(SRCS_RMS)

run: host_single_demo
	./host_single_demo

run_rms: host_single_rms_demo
	./host_single_rms_demo

clean:
	rm -f host_single_demo host_single_rms_demo *.o

.PHONY: all run run_rms clean
