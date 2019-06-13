CXX = clang++
CXXFLAGS = -g -std=c++11 -Wall
LIBS = -lSDL2
DYREC = `llvm-config --cxxflags --ldflags --system-libs --libs core`
BIN = main
SRC = src/main.cpp \
      src/interpreter/cpu.cpp \
      src/mem.cpp \
      src/mbc.cpp \
      src/lcd.cpp \
      src/keypad.cpp \
      src/sound.cpp \
      src/recompiler/dynarec.cpp \
      src/recompiler/translate.cpp \
      src/recompiler/emit.cpp \
      src/recompiler/cache.cpp
OBJ = $(SRC:%.cpp=%.o) 

.PHONY: clean asm check
.SUFFIXES: .t

all: bin
bin: $(OBJ)
	$(CXX) $(LIBS) -o $(BIN) $^
%.o: %.c
	$(CXX) $@ -c $<
clean:
	rm -rf src/*.o src/interpreter/*.o src/recompiler/*.o src/*.s $(BIN) src/*.t
asm: $(SRC)
	$(CXX) $(LIBS) -S -g $^

.cpp.t:
	$(CXX) $(CXXFLAGS) $(LIBS) -DTEST -o $@ $<
test: $(TESTS)
	set -e; $(TESTS:%=./%;)
check:
	$(CXX) -E mem.cpp
