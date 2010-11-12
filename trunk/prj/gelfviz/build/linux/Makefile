CXX=g++
CFLAGS=-c -Wall
RM=rm -f
SRCDIR=../../src
INCS=-I . -I ../../../../inc
BINDIR=../../../../bin
ELFIO=$(BINDIR)/$(SYMBOLIC_TARGET)
UCRT=/usr/lib/libucrt.so.2
objs=gelfviz.o menu.o state.o treeview.o pane.o

OUT=gelfviz

all: $(objs)
	$(CXX) -o $(OUT) $(objs) $(ELFIO) $(UCRT) `pkg-config --libs gtk+-2.0`
	mv $(OUT) $(BINDIR)

install:
	mkdir -p $(DIST_PREFIX)/usr
	mkdir -p $(DIST_PREFIX)/usr/bin
	cp $(BINDIR)/$(OUT) $(DIST_PREFIX)/usr/bin
	
clean:
	$(RM) $(DIST_PREFIX)/usr/bin/$(OUT)
	$(RM) $(objs) $(BINDIR)/$(OUT)

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@ `pkg-config --cflags gtk+-2.0`
