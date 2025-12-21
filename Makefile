## Compiler and flags
CXX := g++
CXXFLAGS := -std=c++11 -g

## Output executable
TARGET := test

## Source layout
SRC_ALL := $(wildcard *.cpp)
# Exclude main and test sources from common object list
COMMON_SRCS := $(filter-out Main.cpp test.cpp,$(SRC_ALL))
COMMON_OBJS := $(COMMON_SRCS:.cpp=.o)

## Valgrind settings
VAL := valgrind
VGFLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --errors-for-leak-kinds=all --error-exitcode=1 -s

## Build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

## Link test binary (depends on test.o and common objects)
$(TARGET): test.o $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

## Default target
all: $(TARGET)

# Convenience targets
.PHONY: all Testing run vt gdb clean

Testing: $(TARGET)

run: $(TARGET)
	./$(TARGET)

vt: $(TARGET)
	$(VAL) $(VGFLAGS) ./$(TARGET) $(ARGS)

gdb: $(TARGET)
	gdb ./$(TARGET)

clean:
	rm -f *.o *.txt $(TARGET) core* vgcore*