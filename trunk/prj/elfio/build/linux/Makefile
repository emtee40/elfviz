CC=gcc
CXX=g++
TARGET=libelfio.so.2.0.0
SYMBOLIC_TARGET=libelfio.so.2.0
CFLAGS=-fPIC -c -Wall
LFLAGS=-shared -Wl,-soname,$(SYMBOLIC_TARGET) -o $(TARGET)
RM=rm -f
SRCDIR=../../src
INCS=-I../../../../inc
BINDIR=../../../../bin
objs=elfio.o \
	phdr.o \
	phdr_entry.o \
	shdr.o \
	shdr_entry.o \
	shdr_rel_entry.o \
	shdr_symtab_entry.o

all: $(objs)
	$(CXX) $(LFLAGS) $?
	ln -s $(TARGET) $(SYMBOLIC_TARGET)
	mv $(TARGET) $(SYMBOLIC_TARGET) $(BINDIR)

clean:
	$(RM) $(objs) $(BINDIR)/$(TARGET) $(BINDIR)/$(SYMBOLIC_TARGET)

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@

