SRCDIR=src
CLIENTSRC=$(SRCDIR)/client
SERVERSRC=$(SRCDIR)/server
UTILSRC=$(SRCDIR)/util

BUILDDIR=build
CFLAGS=-Wall -Werror -std=c99 -g
CC=gcc 

OBJS= \
	  $(BUILDDIR)/client.o \
	  $(BUILDDIR)/server.o \
	  $(BUILDDIR)/connect.o \
	  $(BUILDDIR)/utils.o \
	  $(BUILDDIR)/buffer.o

all: ptyb

ptyb: $(SRCDIR)/ptyb.c $(OBJS)
	$(CC) $(SRCDIR)/ptyb.c $(OBJS) -o ptyb $(CFLAGS)

# Client
$(BUILDDIR)/client.o: $(BUILDDIR) $(CLIENTSRC)/client.c $(CLIENTSRC)/client.h $(BUILDDIR)/connect.o
	$(CC) -c $(CLIENTSRC)/client.c -o $(BUILDDIR)/client.o $(CFLAGS)

$(BUILDDIR)/connect.o: $(BUILDDIR) 
	$(CC) -c $(CLIENTSRC)/connect.c -o $(BUILDDIR)/connect.o $(CFLAGS)

# Server
$(BUILDDIR)/server.o: $(BUILDDIR) $(SERVERSRC)/server.c $(SERVERSRC)/server.h $(BUILDDIR)/buffer.o
	$(CC) -c $(SERVERSRC)/server.c -o $(BUILDDIR)/server.o $(CFLAGS)

buffertest: $(SERVERSRC)/buffertest.c
	./$(BUILDDIR)/buffertest

$(SERVERSRC)/buffertest.c: $(BUILDDIR) $(BUILDDIR)/buffer.o
	$(CC) $(SERVERSRC)/buffertest.c $(BUILDDIR)/buffer.o -o $(BUILDDIR)/buffertest $(CFLAGS)

$(BUILDDIR)/buffer.o: $(BUILDDIR) $(SERVERSRC)/buffer.c $(SERVERSRC)/server.h
	$(CC) -c $(SERVERSRC)/buffer.c -o $(BUILDDIR)/buffer.o $(CFLAGS)

# Utils
utiltest: $(UTILSRC)/utiltest.c
	./$(BUILDDIR)/utiltest

$(UTILSRC)/utiltest.c: $(BUILDDIR) $(BUILDDIR)/utils.o 
	$(CC) $(UTILSRC)/utiltest.c $(BUILDDIR)/utils.o -o $(BUILDDIR)/utiltest $(CFLAGS)

$(BUILDDIR)/utils.o: $(BUILDDIR) $(UTILSRC)/utils.c $(UTILSRC)/utils.h
	$(CC) -c $(UTILSRC)/utils.c -o $(BUILDDIR)/utils.o $(CFLAGS)

$(BUILDDIR):
	mkdir $(BUILDDIR)

clean: 
	rm -rf $(BUILDDIR); rm ptyb
