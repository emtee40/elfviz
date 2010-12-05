include ../../../../build/linux/config.min
CXX=g++
CFLAGS=-c -Wall
RM=rm -f
SRCDIR=../../src
INCS=-I . -I ../../../../inc
BINDIR=../../../../bin
UCRT=/usr/lib/libucrt-2.0.0.so
objs=gelfviz.o menu.o state.o treeview.o pane.o

OUT=gelfviz

all: $(objs)
	$(CXX) -o $(OUT) $(objs) -L$(BINDIR) -l$(EV_TITLE) $(UCRT) `pkg-config --libs gtk+-2.0`
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
