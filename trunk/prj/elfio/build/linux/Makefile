CC=gcc
CXX=g++
CFLAGS=-fPIC -c -Wall
LFLAGS=-shared -o $(TARGET)
RM=rm -f
SRCDIR=../../src
INCS=-I../../../../inc
BINDIR=../../../../bin
objs=elfio.o \
	elf_buffer.o \
	phdr.o \
	phdr_entry.o \
	shdr.o \
	shdr_entry.o \
	shdr_rel_entry.o \
	shdr_symtab_entry.o

all: $(objs)
	$(CXX) $(LFLAGS) $?
	mv $(TARGET) $(BINDIR)

install:
	mkdir -p $(DIST_PREFIX)/usr
	mkdir -p $(DIST_PREFIX)/usr/lib
	mkdir -p $(DIST_PREFIX)/usr/include
	cp $(BINDIR)/$(TARGET) $(DIST_PREFIX)/usr/lib
	mkdir -p $(DIST_PREFIX)/usr/include/
	cp ../../../../inc/elfio/*.h $(DIST_PREFIX)/usr/include/elfio

clean:
	$(RM) $(DIST_PREFIX)/usr/lib/$(TARGET)
	rm -rf $(DIST_PREFIX)/usr/include/elfio
	$(RM) $(objs) $(BINDIR)/$(TARGET)

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@

