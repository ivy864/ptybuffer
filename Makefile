SRCDIR=src
BUILDDIR=build
CFLAGS=-Wall -Werror -std=c99
CC=gcc

all: ptyb

ptyb: $(SRCDIR)/ptyb.c
	$(CC) $(SRCDIR)/ptyb.c -o ptyb $(CFLAGS)

clean: 
	rm ptyb
