CXX = clang++
CXXFLAGS = -O0 -g -std=c++11 -Wall
LIBS = -lSDL2
DYREC = `llvm-config --cxxflags --ldflags --system-libs --libs core`
BIN = emulator
SRC = main.cpp cpu.cpp mem.cpp mbc.cpp lcd.cpp keypad.cpp sound.cpp dynarec.cpp traverse.cpp emit.cpp cache.cpp
TESTS = mem.t
OBJ = $(SRC:%.cpp=%.o) 

.PHONY: clean asm check
.SUFFIXES: .t

all: bin
bin: $(OBJ)
	$(CXX) $(LIBS) -o $(BIN) $^
%.o: %.c
	$(CXX) $@ -c $<
clean:
	rm -rf *.o *.s $(BIN) *.t
asm: $(SRC)
	$(CXX) $(LIBS) -S -g $^

.cpp.t:
	$(CXX) $(CXXFLAGS) $(LIBS) -DTEST -o $@ $<
test: $(TESTS)
	set -e; $(TESTS:%=./%;)
check:
	$(CXX) -E mem.cpp
