CXX=g++
CFLAGS=-c -Wall
RM=rm -f
SRCDIR=../../src
INCS=-I . -I ../../../../inc
BINDIR=../../../../bin
ELFIO=$(BINDIR)/libelfio.so.2.0
UCRT=/usr/lib/libucrt.so.1
objs=gelfviz.o menu.o state.o

OUT=gelfviz

all: $(objs)
	$(CXX) -o $(OUT) $(objs) $(ELFIO) $(UCRT) `pkg-config --libs gtk+-2.0`
	mv $(OUT) $(BINDIR)

clean:
	$(RM) $(objs) $(BINDIR)/$(OUT)

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@ `pkg-config --cflags gtk+-2.0`
