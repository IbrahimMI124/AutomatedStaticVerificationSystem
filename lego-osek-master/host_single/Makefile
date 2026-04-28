CC ?= gcc
CFLAGS ?= -std=c11 -O0 -g -Wall -Wextra -Werror \
	-Wno-unused-but-set-variable -Wno-unused-variable

INCLUDES = -I./include -I..

SRCS = \
	../nxtgt.c \
	stubs.c \
	osek_sim.c \
	main.c

all: host_single_demo

host_single_demo: $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SRCS)

run: host_single_demo
	./host_single_demo

clean:
	rm -f host_single_demo *.o

.PHONY: all run clean
