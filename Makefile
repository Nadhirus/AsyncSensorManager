.POSIX:
CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -O3 -mcx16 -pthread -Iinclude
LDFLAGS = -pthread
#LDLIBS  = -latomic

# Define source and header files
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
C = $(wildcard $(SRC_DIR)/*.c)
H = $(wildcard $(INC_DIR)/*.h)
PREAMBULE_SRCS := $(SRC_DIR)/preambule.c
POSIX_SRCS := $(filter-out $(SRC_DIR)/preambule.c $(SRC_DIR)/acquisitionManagerAtomic.c $(SRC_DIR)/acquisitionManagerTestAndSet.c, $(C))
ATOMIC_SRCS := $(filter-out $(SRC_DIR)/preambule.c $(SRC_DIR)/acquisitionManagerPOSIX.c $(SRC_DIR)/acquisitionManagerTestAndSet.c, $(C))
TESTANDSET_SRCS := $(filter-out $(SRC_DIR)/preambule.c $(SRC_DIR)/acquisitionManagerPOSIX.c $(SRC_DIR)/acquisitionManagerAtomic.c, $(C))

# Object files
PREAMBULE_OBJS = $(PREAMBULE_SRCS:.c=.o)
POSIX_OBJS = $(POSIX_SRCS:.c=.o)
ATOMIC_OBJS = $(ATOMIC_SRCS:.c=.o)
TESTANDSET_OBJS = $(TESTANDSET_SRCS:.c=.o)

# Targets
all: $(BIN_DIR)/preambule $(BIN_DIR)/multitaskingAccumulatorPosix $(BIN_DIR)/multitaskingAccumulatorAtomic $(BIN_DIR)/multitaskingAccumulatorTestAndSet

$(BIN_DIR)/preambule: $(PREAMBULE_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(PREAMBULE_OBJS)

$(BIN_DIR)/multitaskingAccumulatorPosix: $(POSIX_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(POSIX_OBJS)

$(BIN_DIR)/multitaskingAccumulatorAtomic: $(ATOMIC_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(ATOMIC_OBJS)

$(BIN_DIR)/multitaskingAccumulatorTestAndSet: $(TESTANDSET_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(TESTANDSET_OBJS)

# Clean up generated files
clean:
	rm -f $(BIN_DIR)/* $(PREAMBULE_OBJS) $(POSIX_OBJS) $(ATOMIC_OBJS) $(TESTANDSET_OBJS)

# Run the executables
runpreambule: clean $(BIN_DIR)/preambule
	./$(BIN_DIR)/preambule

runposix: clean $(BIN_DIR)/multitaskingAccumulatorPosix
	./$(BIN_DIR)/multitaskingAccumulatorPosix

runatomic: clean $(BIN_DIR)/multitaskingAccumulatorAtomic
	./$(BIN_DIR)/multitaskingAccumulatorAtomic

runtestandset: clean $(BIN_DIR)/multitaskingAccumulatorTestAndSet
	./$(BIN_DIR)/multitaskingAccumulatorTestAndSet

# Ensure the bin directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
