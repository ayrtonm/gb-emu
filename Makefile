CXX = g++
override CXXFLAGS := -g -std=c++17 -Wall $(CXXFLAGS)
LIBS = -lSDL2 -ljitplus -ljit
BIN = gb-emu
SRC = main.cpp mem.cpp mbc.cpp lcd.cpp keypad.cpp cpu.cpp sound.cpp throttle.cpp states.cpp dynarec.cpp translate.cpp emit.cpp cache.cpp target_function.cpp
OBJ = $(SRC:%.cpp=%.o)
VPATH = src:src/interpreter:src/recompiler

.PHONY: clean

bin: $(OBJ)
	$(CXX) $(LIBS) -o $(BIN) $^
clean:
	-rm $(OBJ) $(BIN)
