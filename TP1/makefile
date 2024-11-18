# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -w  # -w will suppress all warnings

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

# Include dependencies
-include $(DEP)

# Clean up the build
clean:
	rm -f $(OBJ) $(TARGET) $(DEP)

# PHONY targets to prevent conflicts with files of the same name
.PHONY: all clean

