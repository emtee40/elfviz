CXX=g++
CFLAGS=-c -O2 -Wall
RM=rm -f
SRCDIR=../../src
INCS=-I . -I ../../../../inc
BINDIR=../../../../bin
ELFIO=$(BINDIR)/$(SYMBOLIC_TARGET)
objs=elfviz.o \
	cmdaction.o \
	cmdparam.o \
	vizcmd.o \
	elfstack.o \
	argparse.o

OUT=elfviz

all: $(objs)
	$(CXX) -o $(OUT) $(objs) $(ELFIO)
	mv $(OUT) $(BINDIR)

install:
	mkdir -p $(DIST_PREFIX)/usr
	mkdir -p $(DIST_PREFIX)/usr/bin
	cp $(BINDIR)/$(OUT) $(DIST_PREFIX)/usr/bin

clean:
	$(RM) $(DIST_PREFIX)/usr/bin/$(OUT)
	$(RM) $(objs) $(BINDIR)/$(OUT)

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@
