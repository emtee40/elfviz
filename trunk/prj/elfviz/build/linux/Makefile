MAKE=make
CC=gcc
LD=ld
CFLAGS=-c -O2 -Wall
USR_CFLAGS=
RM=rm -f
INCDIR=../../inc
SRCDIR=../../src
INCS=-I . -I ../../../../inc
LIBDIR=../../../../lib
BINDIR=../../../../bin
RUNTIME=/usr/lib/libucrt.so.1
objs=elfviz.o vizcmd.o
OUT=elfviz

elfviz: $(objs)
	$(CC) -o $(OUT) -L$(LIBDIR) $(objs) -l$(RUNTIME) -lc
	mv $(OUT) $(BINDIR)
debug:
	$(MAKE) USR_CFLAGS=-g
clean:
	$(RM) $(objs) $(BINDIR)/$(OUT)

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@
