MAKE=make
CPP=g++
LD=ld
CFLAGS=-c -O2 -Wall
USR_CFLAGS=
RM=rm -f
INCDIR=../../inc
SRCDIR=../../src
INCS=-I . -I ../../../../inc
LIBDIR=../../../../lib
BINDIR=../../../../bin
ELFIO=$(LIBDIR)/libelfio.a
RUNTIME=/usr/lib/libucrt.so.1
objs=elfviz.o \
	cmdaction.o \
	cmdparam.o \
	vizcmd.o \
	argparse.o

OUT=elfviz

elfviz: $(objs)
	$(CPP) -o $(OUT) -L$(LIBDIR) $(objs) $(ELFIO) $(RUNTIME)
	mv $(OUT) $(BINDIR)
debug:
	$(MAKE) USR_CFLAGS=-g
clean:
	$(RM) $(objs) $(BINDIR)/$(OUT)

%.o: $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@
