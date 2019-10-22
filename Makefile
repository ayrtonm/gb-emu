CXX = g++
override CXXFLAGS := -g -std=c++17 -Wall $(CXXFLAGS)
LIBS_DYNAREC = -ljitplus -ljit
LIBS = -lSDL2
BIN = gb-emu
SRC = main.cpp mem.cpp mbc.cpp lcd.cpp keypad.cpp cpu.cpp sound.cpp throttle.cpp states.cpp dynarec.cpp cache.cpp
OBJ = $(SRC:%.cpp=%.o)
VPATH = src:src/interpreter:src/recompiler

.PHONY: clean

bin: $(OBJ)
ifneq (,$(findstring -DDYNAREC_CPU, $(CXXFLAGS)))
	$(CXX) $(LIBS) $(LIBS_DYNAREC) -o $(BIN) $^
else
	$(CXX) $(LIBS) -o $(BIN) $^
endif

clean:
	-rm $(OBJ) $(BIN)
