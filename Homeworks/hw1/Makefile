CFLAGS=-g -Wall -std=c99

all: average powerMod

average: average.c
	cc $(CFLAGS) -o average average.c

powerMod: powerMod.c
	cc $(CFLAGS) -o powerMod powerMod.c

clean:
	rm -rf *.o *~ average powerMod
