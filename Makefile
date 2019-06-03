XX = g++
CXXFLAGS = -g -std=c++11 -Wall
LIBS = -lSDL2
BIN = main
SRC = main.cpp cpu.cpp mem.cpp mbc.cpp lcd.cpp keypad.cpp sound.cpp
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
