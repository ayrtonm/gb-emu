CXX = g++
override CXXFLAGS := -g -std=c++11 -Wall $(CXXFLAGS)
LIBS = -lSDL2
BIN = gb-emu
SRC = main.cpp mem.cpp mbc.cpp lcd.cpp keypad.cpp cpu.cpp sound.cpp throttle.cpp states.cpp
OBJ = $(SRC:%.cpp=%.o)
VPATH = src:src/interpreter

.PHONY: clean

bin: $(OBJ)
	$(CXX) $(LIBS) -o $(BIN) $^
clean:
	-rm $(OBJ) $(BIN)
