# Compiler and flags
CC = gcc
CFLAGS = -g  # Default flags without warnings
DFLAGS = -DDEBUG        # Debug-specific flag
DEBUG_FLAGS = -Wall -Wextra  # Flags for enabling warnings in debug mode

# Files
SRC = multitaskingAccumulator.c acquisitionManagerPOSIX.c displayManager.c messageAdder.c msg.c sensorManager.c display.c
OBJ = $(SRC:.c=.o)
TARGET = program

# Dependency files
DEP = $(SRC:.c=.d)

# Build rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	$(CC) -MM $(CFLAGS) $< > $@

# Debug build
debug: CFLAGS += $(DFLAGS) $(DEBUG_FLAGS)  # Add debug flags and warning flags
debug: clean $(TARGET)      # Build clean and then compile with debug

# Include dependencies
-include $(DEP)

# Clean up the build
clean:
	rm -f $(OBJ) $(TARGET) $(DEP)

# PHONY targets to prevent conflicts with files of the same name
.PHONY: all clean debug

