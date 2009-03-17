CXX=g++
CFLAGS=-c -O2 -Wall
RM=rm -f
SRCDIR=../../src
INCS=-I . -I ../../../../inc
BINDIR=../../../../bin
ELFIO=$(BINDIR)/libelfio.so.2.0
UCRT=/usr/lib/libucrt.so.1
objs=elf2txt.o argument.o

OUT=elf2txt

all: $(objs)
	$(CXX) -o $(OUT) $(objs) $(ELFIO) $(UCRT)
	mv $(OUT) $(BINDIR)

clean:
	$(RM) $(objs) $(BINDIR)/$(OUT)

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@
