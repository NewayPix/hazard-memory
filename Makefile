CXX         = g++
SRC_DIR     = src
INCLUDES    = $(shell pkg-config --cflags sdl2) -I  $(SRC_DIR)
CXXFLAGS    = -w $(INCLUDES) -g -Wall -Wextra -Werror -DDEBUG -std=c++17
LFLAGS      = -lSDL2 -lm -lSDL2_gfx
MAIN        = $(SRC_DIR)/main.cpp
SRCS        = $(shell find $(SRC_DIR) -name "*.cpp" -type f)
LIB_SRCS    = $(filter-out $(MAIN), $(SRCS))
TESTS       = $(wildcard tests/*.cpp)
EXAMPLES    = $(wildcard examples/*.cpp)
RUN_EXAMPLE = 1
LIB_OBJS    = $(LIB_SRCS:%.cpp=%.o)
TESTS_OBJS  = $(TESTS:%.cpp=%.bin)
OBJS        = $(SRCS:%.cpp=%.o)
BIN_NAME    = hazard-memory.bin


all: $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LFLAGS) -o $(BIN_NAME)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

# running examples easily
FORCE:
examples/%.cpp: $(LIB_OBJS) FORCE
	$(CXX) $(LIB_OBJS) $@ $(CXXFLAGS) $(LFLAGS) -o $(@:%.cpp=%.bin)
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

tests/%.bin: tests/%.cpp $(LIB_OBJS)
	$(CXX) $(LFLAGS) $(CXXFLAGS) $(LIB_OBJS) $< -o $@
	@./$@
	@echo -- $< [ok]

run: all
	./$(BIN_NAME)

clean:
	find . -iname '*.o' -or -iname '*.bin' | xargs rm -fv

tags:
	find . -type f -name "*.*pp" | xargs etags -a

# for debug purposes
vars:
	@echo CXX      = $(CXX)
	@echo CXXFLAGS = $(CXXFLAGS)
	@echo LFLAGS   = $(LFLAGS)
	@echo INCLUDES = $(INCLUDES)
	@echo SRC_DIR  = $(SRC_DIR)
	@echo SRCS     = $(SRCS)
	@echo EXAMPLES = $(EXAMPLES)
	@echo LIB_SRCS = $(LIB_SRCS)
	@echo LIB_OBJS = $(LIB_OBJS)
	@echo OBJS     = $(OBJS)
	@echo BIN_NAME = $(BIN_NAME)


.PHONY: vars tags
