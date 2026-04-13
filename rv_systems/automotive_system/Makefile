CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -O0 -g

TARGET = automotive_system
HS_SPEC = automotive_system.hs
C_MONITOR = automotive_system.c
H_MONITOR = automotive_system.h
H_MONITOR_TYPES = automotive_system_types.h
MAIN = main.c
COMMON = ../common/nondet.c

all: $(TARGET)

.PHONY: all run clean

$(C_MONITOR) $(H_MONITOR): $(HS_SPEC)
	runhaskell $(HS_SPEC)

$(TARGET): $(C_MONITOR) $(MAIN) $(COMMON)
	$(CC) $(CFLAGS) -I. -I../common $(C_MONITOR) $(MAIN) $(COMMON) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(C_MONITOR) $(H_MONITOR) $(H_MONITOR_TYPES)
	rm -f *.o *.a *.so *.dylib *.dll *.exe
	rm -f *.hi *.dyn_hi *.dyn_o *.hie *.prof *.hp *.eventlog
	rm -f core vgcore.* gmon.out
	rm -f *~ \#*\# *.swp *.swo *.tmp *.bak .DS_Store
