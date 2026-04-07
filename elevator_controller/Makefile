CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -O0 -g

BIN := elevator
SRC := system.c elevator.c nondet.c monitor_inputs.c

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -I. -o $@ $(SRC)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)
	rm -f *.o *.a *.so *.dylib *.dll *.exe
	rm -f *.hi *.dyn_hi *.dyn_o *.hie *.prof *.hp *.eventlog
	rm -f core vgcore.* gmon.out
	rm -f *~ \#*\# *.swp *.swo *.tmp *.bak .DS_Store
	# Haskell monitor build artifacts (if present)
	rm -f ElevatorMon ElevatorMon.o ElevatorMon.hi

.PHONY: all run clean
