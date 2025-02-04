CC = gcc
CFLAGS += -Wall -g -O2
EXTRA_CFLAGS ?= -ffreestanding -fno-stack-protector -nostdlib -nostdinc -static -m32

all: hexconv

hexconv: hexconv.c
	$(CC) $(EXTRA_CFLAGS) $(CFLAGS) hexconv.c -o hexconv

tar:
	tar czf osy07.tgz hexconv.c
