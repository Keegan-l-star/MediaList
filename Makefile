## Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -g

## Output executables
TARGET := test
MAIN := list

## Source layout
SRC_ALL := $(wildcard *.cpp)
# Exclude main and test sources from common object list
COMMON_SRCS := $(filter-out List.cpp test.cpp,$(SRC_ALL))
COMMON_OBJS := $(COMMON_SRCS:.cpp=.o)

## Valgrind settings
VAL := valgrind
VGFLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --errors-for-leak-kinds=all --error-exitcode=1 -s

## Build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

## Link test binary (depends on test.o and common objects)
$(TARGET): test.o $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lgtest -lgtest_main -pthread

## Link main program with UI
$(MAIN): List.o $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

## Default target
all: $(TARGET) $(MAIN)

# Convenience targets
.PHONY: all Testing run vt gdb clean desktop

Testing: $(TARGET)

run: $(TARGET)
	./$(TARGET)

desktop: $(MAIN)
	@echo '[Desktop Entry]' > MediaList.desktop
	@echo 'Type=Application' >> MediaList.desktop
	@echo 'Name=MediaList' >> MediaList.desktop
	@echo 'Exec=gnome-terminal -- bash -c "cd $(CURDIR) && ./$(MAIN); read -p \"Press Enter to close...\""' >> MediaList.desktop
	@echo 'Terminal=false' >> MediaList.desktop
	@chmod +x MediaList.desktop
	@echo "Created MediaList.desktop - you can double-click it to run the program"

vt: $(TARGET)
	$(VAL) $(VGFLAGS) ./$(TARGET) $(ARGS)

gdb: $(TARGET)
	gdb ./$(TARGET)

clean:
	rm -f *.o *.txt $(TARGET) core* vgcore*