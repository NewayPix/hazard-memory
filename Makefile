CC = g++
SRC_DIR = src
INCLUDES = $(shell pkg-config --cflags sdl2) -I $(SRC_DIR)
CFLAGS = -w $(INCLUDES) -g
LFLAGS = -lSDL2 -lm
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TESTS = $(wildcard tests/*.cpp)
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
	./$(@:%.cpp=%.bin)

tests: $(TESTS_OBJS)

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


.PHONY: debug
