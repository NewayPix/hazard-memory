CC = g++
INCLUDES = $(shell pkg-config --cflags sdl2)
CFLAGS = -w $(INCLUDES)
LFLAGS = -lSDL2
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:%.cpp=%.o)
BIN_NAME = hazard-memory.bin


all: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LFLAGS) -o $(BIN_NAME)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

# running examples easily
FORCE:
examples/%: examples/%.cpp FORCE
	$(CC) $< $(CFLAGS) $(LFLAGS) -o $(<:%.cpp=%.bin)
	./$(<:%.cpp=%.bin)

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
