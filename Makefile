CXX = g++
SRC_DIR = src
INCLUDES = $(shell pkg-config --cflags sdl2) -I  $(SRC_DIR)
CXXFLAGS = -w $(INCLUDES) -g -Wall -Wextra -Werror
LFLAGS = -lSDL2 -lm
SRCS = $(shell find $(SRC_DIR) -name "*.cpp" -type f)
TESTS = $(wildcard tests/*.cpp)
EXAMPLES = $(wildcard examples/*.cpp)
RUN_EXAMPLE=1
TESTS_OBJS = $(TESTS:%.cpp=%.bin)
OBJS = $(SRCS:%.cpp=%.o)
BIN_NAME = hazard-memory.bin


all: $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LFLAGS) -o $(BIN_NAME)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

# running examples easily
FORCE:
examples/%.cpp: FORCE
	$(CXX) $@ $(CXXFLAGS) $(LFLAGS) -o $(@:%.cpp=%.bin)
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
	@$(CXX) $(LFLAGS) $(CXXFLAGS) $< -o $@
	@./$@
	@rm -f $@
	@echo -- $< [ok]

run: all
	./$(BIN_NAME)

clean:
	find . -iname '*.o' -or -iname '*.bin' | xargs rm -fv

# for debug purposes
vars:
	@echo CXX      = $(CXX)
	@echo CXXFLAGS = $(CXXFLAGS)
	@echo LFLAGS   = $(LFLAGS)
	@echo INCLUDES = $(INCLUDES)
	@echo SRC_DIR  = $(SRC_DIR)
	@echo SRCS     = $(SRCS)
	@echo OBJS     = $(OBJS)
	@echo BIN_NAME = $(BIN_NAME)


.PHONY: vars
