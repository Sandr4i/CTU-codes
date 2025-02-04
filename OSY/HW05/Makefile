CC = gcc
CFLAGS += -Wall -g -O2
EXTRA_CFLAGS ?= -fsanitize=address -fno-omit-frame-pointer

all: prod-cons

prod-cons: prod-cons.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) prod-cons.c -o prod-cons

tar:
	tar czf osy05.tgz Makefile *.[ch]

 
