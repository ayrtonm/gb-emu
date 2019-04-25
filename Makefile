CXX = g++
CXXFLAGS = -g -std=c++11 -Wall
LIBS = -lSDL
BIN = main
SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o) 

all: $(OBJ)
	$(CXX) $(LIBS) -o $(BIN) $^
%.o: %.c
	$(CXX) $@ -c $<
clean:
	rm -rf *.o *.s $(BIN)
asm: $(SRC)
	$(CXX) $(LIBS) -S -g $^
