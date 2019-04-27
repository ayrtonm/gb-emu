CXX = g++
CXXFLAGS = -g -std=c++11 #-Wall
LIBS = -lSDL
BIN = main
SRC = main.cpp cpu.cpp mem.cpp lcd.cpp SDL_rotozoom.cpp
OBJ = $(SRC:%.cpp=%.o) 

all: clean bin
bin: $(OBJ)
	$(CXX) $(LIBS) -o $(BIN) $^
%.o: %.c
	$(CXX) $@ -c $<
clean:
	rm -rf *.o *.s $(BIN)
asm: $(SRC)
	$(CXX) $(LIBS) -S -g $^
