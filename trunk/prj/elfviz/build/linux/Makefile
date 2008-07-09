CPP=g++
CFLAGS=-c -O2 -Wall
USR_CFLAGS=
RM=rm -f
SRCDIR=../../src
INCS=-I . -I ../../../../inc
BINDIR=../../../../bin
ELFIO=/usr/lib/libelfio.so.1
objs=elfviz.o \
	cmdaction.o \
	cmdparam.o \
	vizcmd.o \
	argparse.o

OUT=elfviz

elfviz: $(objs)
	$(CPP) -o $@ $(objs) $(ELFIO)
	mv $@ $(BINDIR)

install: elfviz
	cp $(BINDIR)/$(OUT) /usr/bin

debug:
	$(MAKE) USR_CFLAGS=-g

clean:
	$(RM) $(objs) $(BINDIR)/$(OUT)
	$(RM) /usr/bin/$(OUT)make

%.o: $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@
