CXX = clang++
CXXFLAGS = -O0 -g -std=c++11 -Wall #$(DYREC)
LIBS = -lSDL2
DYREC = `llvm-config --cxxflags --ldflags --system-libs --libs core`
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
