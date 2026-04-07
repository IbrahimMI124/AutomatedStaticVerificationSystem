# compiler
CC = gcc
CFLAGS = -std=c99 -Wall

# files
TARGET = engine
HS_SPEC = engine.hs
C_MONITOR = engine.c
H_MONITOR = engine.h
H_MONITOR_TYPES = engine_types.h
MAIN = main.c

all: $(TARGET)

.PHONY: all run clean

# generate Copilot monitor
$(C_MONITOR) $(H_MONITOR): $(HS_SPEC)
	runhaskell $(HS_SPEC)

# compile program
$(TARGET): $(C_MONITOR) $(MAIN)
	$(CC) $(CFLAGS) $(C_MONITOR) $(MAIN) -o $(TARGET)

# run demo
run: $(TARGET)
	./$(TARGET)

# remove generated files
clean:
	rm -f $(TARGET) $(C_MONITOR) $(H_MONITOR) $(H_MONITOR_TYPES)
	rm -f *.o *.a *.so *.dylib *.dll *.exe
	rm -f *.hi *.dyn_hi *.dyn_o *.hie *.prof *.hp *.eventlog
	rm -f core vgcore.* gmon.out
	rm -f *~ \#*\# *.swp *.swo *.tmp *.bak .DS_Store

