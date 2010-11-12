CXX=g++
CFLAGS=-c -O2 -Wall
RM=rm -f
SRCDIR=../../src
INCS=-I . -I ../../../../inc
BINDIR=../../../../bin
ELFIO=$(BINDIR)/$(SYMBOLIC_TARGET)
UCRT=/usr/lib/libucrt.so.2
UCXML=/usr/lib/libucxml.so.2
objs=elf2txt.o argument.o

OUT=elf2txt

all: $(objs)
	$(CXX) -o $(OUT) $(objs) $(ELFIO) $(UCXML) $(UCRT)
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
