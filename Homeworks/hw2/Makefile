#define more variables so it is easier to make changes
CC=gcc
CFLAGS=-g -Wall -std=c99
TARGETS=collatz  powerMod-r

all: $(TARGETS)

collatz: collatz.c
	$(CC) $(CFLAGS) -o $@ $@.c

powerMod-r: powerMod-r.c
	$(CC) $(CFLAGS) -o $@ $@.c

clean:
	rm -rf *.o *~ $(TARGETS)
