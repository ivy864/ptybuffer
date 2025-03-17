SRCDIR=src
CLIENTSRC=$(SRCDIR)/client
BUILDDIR=build
CFLAGS=-Wall -Werror -std=c99 -g
CC=gcc

OBJS= \
	  $(BUILDDIR)/client.o

all: ptyb

ptyb: $(SRCDIR)/ptyb.c $(OBJS)
	$(CC) $(SRCDIR)/ptyb.c $(OBJS) -o ptyb $(CFLAGS)

$(BUILDDIR)/client.o:  $(BUILDDIR) $(CLIENTSRC)/client.c $(CLIENTSRC)/client.h
	$(CC) -c $(CLIENTSRC)/client.c -o $(BUILDDIR)/client.o $(CFLAGS)

$(BUILDDIR):
	mkdir $(BUILDDIR)

clean: 
	rm ptyb && rm -rf $(BUILDDIR)
