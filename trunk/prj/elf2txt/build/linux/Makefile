include ../../../../build/linux/config.min
CXX=g++
CFLAGS=-c -O2 -Wall
RM=rm -f
SRCDIR=../../src
INCS=-I . -I ../../../../inc
BINDIR=../../../../bin
UCRT=/usr/lib/libucrt-2.0.0.so
UCXML=/usr/lib/libucxml-2.0.0.so
objs=elf2txt.o argument.o

OUT=elf2txt

all: $(objs)
	$(CXX) -o $(OUT) $(objs) -L$(BINDIR) -l$(EV_TITLE) $(UCXML) $(UCRT)
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
