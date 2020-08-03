CC = g++
SRC_DIR = src
INCLUDES = $(shell pkg-config --cflags sdl2) -I $(SRC_DIR)
CFLAGS = -w $(INCLUDES) -g -Wall -Wextra -Werror
LFLAGS = -lSDL2 -lm
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TESTS = $(wildcard tests/*.cpp)
EXAMPLES = $(wildcard examples/*.cpp)
RUN_EXAMPLE=1
TESTS_OBJS = $(TESTS:%.cpp=%.bin)
OBJS = $(SRCS:%.cpp=%.o)
BIN_NAME = hazard-memory.bin


all: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LFLAGS) -o $(BIN_NAME)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

# running examples easily
FORCE:
examples/%.cpp: FORCE
	$(CC) $@ $(CFLAGS) $(LFLAGS) -o $(@:%.cpp=%.bin)
	$(if $(RUN_EXAMPLE), ./$(@:%.cpp=%.bin))

examples: $(EXAMPLES)

tests: $(TESTS_OBJS)

check:
	@echo "-- Test everything"
	@echo "-- Unit Testing"
	@make tests
	@echo "-- Compile examples"
	@make examples RUN_EXAMPLE=
	@echo "-- Compile main project"
	@make

tests/%.bin: tests/%.cpp
	@$(CC) $(LFLAGS) $(CFLAGS) $< -o $@
	@./$@
	@rm -f $@
	@echo -- $< [ok]

run: all
	./$(BIN_NAME)

clean:
	find . -iname '*.o' -or -iname '*.bin' | xargs rm -fv

# for debug purposes
vars:
	@echo CC       = $(CC)
	@echo CFLAGS   = $(CFLAGS)
	@echo LFLAGS   = $(LFLAGS)
	@echo SRC_DIR  = $(SRC_DIR)
	@echo SRCS     = $(SRCS)
	@echo OBJS     = $(OBJS)
	@echo BIN_NAME = $(BIN_NAME)


.PHONY: vars
