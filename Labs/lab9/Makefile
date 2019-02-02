SRCS = computer-printer.c
TARGETS = $(patsubst %.c,%,$(SRCS))
CC = gcc
CFLAGS = -Wall -g -pthread

all : $(TARGETS)

clean: 
	@rm -f $(OBJS) $(TARGETS)
