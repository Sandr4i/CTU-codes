CC=gcc
CFLAGS+=-Wall -g -O2
EXTRA_CFLAGS ?= -fsanitize=address -fno-omit-frame-pointer

all: prod-cons

prod-cons:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) prod-cons.c -o prod-cons

tar:
    tar czf osy05.tgz Makefile *.[ch]
# all: forkpipe nsd
 
# forkpipe: forkpipe.c
#     $(CC) $(CFLAGS) $(EXTRA_CFLAGS) forkpipe.c -o forkpipe
 
# nsd: nd.o nsd.o nsd_main.c
#     $(CC) $(CFLAGS) $(EXTRA_CFLAGS) nsd_main.c nd.o nsd.o -o nsd
 
# nd.o: nd.c nd.h
#     $(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) nd.c -o nd.o
 
# nsd.o: nsd.c nsd.h
#     $(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) nsd.c -o nsd.o
 
