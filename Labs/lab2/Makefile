# This is a comment line

#define more variables so it is easier to make changes
CC=gcc
CFLAGS=-g -Wall -std=c99
TARGETS=power ex11-for ex12-factorial catalan

all: $(TARGETS)

power: power.c
	$(CC) $(CFLAGS) -o $@ $@.c

ex11-for: ex11-for.c
	$(CC) $(CFLAGS) -o $@ $@.c

ex12-factorial: ex12-factorial.c
	$(CC) $(CFLAGS) -o $@ $@.c

catalan: catalan.c
	$(CC) $(CFLAGS) -o $@ $@.c

clean:
	rm -rf *.o *~ $(TARGETS)
