CXX = g++
CXXFLAGS = -g -std=c++11 -Wall
LIBS = -lSDL2
BIN = emulator
SRC = main.cpp cpu.cpp mem.cpp lcd.cpp dynarec.cpp traverse.cpp emit.cpp cache.cpp
OBJ = $(SRC:%.cpp=%.o) 

all: bin
bin: $(OBJ)
	$(CXX) $(LIBS) -o $(BIN) $^
%.o: %.c
	$(CXX) $@ -c $<
clean:
	rm -rf *.o *.s $(BIN)
asm: $(SRC)
	$(CXX) $(LIBS) -S -g $^
test: all
	./test_assembler.sh
