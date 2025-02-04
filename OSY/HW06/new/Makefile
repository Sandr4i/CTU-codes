CC = gcc
CFLAGS = -Wall -g -O2
EXTRA_CFLAGS ?= -fsanitize=address -fno-omit-frame-pointer

SRCS = factory.c
OBJS = $(SRCS:.c=.o)

all: factory

factory: $(OBJS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(OBJS) -o $@ -pthread

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $< -o $@

tar:
	tar czf osy06.tgz Makefile $(SRCS)

clean:
	rm -f factory $(OBJS)

