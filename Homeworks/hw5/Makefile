# study the new varabiles and rules
CC=gcc
CFLAGS=-g -Wall -std=c99
TARGETS=runpipeline

all: $(TARGETS)

#Static Pattern Rules
$(TARGETS): % : %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o *.out *~ $(TARGETS) counts*.txt
