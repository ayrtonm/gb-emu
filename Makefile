CC = gcc
CFLAGS = #-g -O3 -fomit-frame-pointer
SRCS = main.c mem.c z80.c gpu.c input.c
OBJS = $(SRCS:.c=.o)
LIBS = -lSDL -lm #-lwiringPi
MAIN = main


all: $(MAIN)
	@echo emulator has been compiled
$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(MAIN)
.c.o:
	$(CC) -c  $< -o $@
clean:
	rm -rf *.o *~ $(MAIN)
