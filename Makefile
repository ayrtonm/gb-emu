CC = gcc
CFLAGS = -g #-O3
SRCS = main.c mem.c cpu.c debug.c
OBJS = $(SRCS:.c=.o)
LIBS =
MAIN = main

all: $(MAIN)
	@echo emulator has been compiled
$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(MAIN)
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf *.o *~ $(MAIN)
