SRCDIR=src
CLIENTSRC=$(SRCDIR)/client
SERVERSRC=$(SRCDIR)/server

BUILDDIR=build
CFLAGS=-Wall -Werror -std=c99 -g
CC=gcc

OBJS= \
	  $(BUILDDIR)/client.o \
	  $(BUILDDIR)/server.o \
	  $(BUILDDIR)/connect.o \
	

CLIENTOBJS= \

all: ptyb

ptyb: $(SRCDIR)/ptyb.c $(OBJS)
	$(CC) $(SRCDIR)/ptyb.c $(OBJS) -o ptyb $(CFLAGS)

$(BUILDDIR)/client.o: $(BUILDDIR) $(CLIENTSRC)/client.c $(CLIENTSRC)/client.h $(BUILDDIR)/connect.o
	$(CC) -c $(CLIENTSRC)/client.c  -o $(BUILDDIR)/client.o $(CFLAGS)

$(BUILDDIR)/connect.o: $(BUILDDIR) 
	$(CC) -c $(CLIENTSRC)/connect.c -o $(BUILDDIR)/connect.o $(CFLAGS)

$(BUILDDIR)/server.o: $(BUILDDIR) $(SERVERSRC)/server.c $(SERVERSRC)/server.h
	$(CC) -c $(SERVERSRC)/server.c  -o $(BUILDDIR)/server.o $(CFLAGS)


$(BUILDDIR):
	mkdir $(BUILDDIR)

clean: 
	rm -rf $(BUILDDIR) && rm ptyb
