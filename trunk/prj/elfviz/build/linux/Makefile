CXX=g++
CFLAGS=-c -O2 -Wall
RM=rm -f
SRCDIR=../../src
INCS=-I . -I ../../../../inc
BINDIR=../../../../bin
ELFIO=$(BINDIR)/libelfio.so.2.0
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

clean:
	$(RM) $(objs) $(BINDIR)/$(OUT)

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@
