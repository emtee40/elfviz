CPP=g++
TARGET=libelfio.so.2.0.0
SYMBOLIC_TARGET=libelfio.so.2.0
CFLAGS=-fPIC -c -Wall
USR_CFLAGS=
LFLAGS=-shared -Wl,-soname,$(SYMBOLIC_TARGET) -o $(TARGET)
RM=rm -f
SRCDIR=../../src
INCS=-I../../../../inc
BINDIR=../../../../bin
USR_LIB=/usr/lib
objs=elfio.o \
	phdr.o \
	phdr_entry.o \
	shdr.o \
	shdr_entry.o \
	shdr_rel_entry.o \
	shdr_symtab_entry.o

elfio: $(objs)
	$(CPP) $(LFLAGS) $?
	mv $(TARGET) $(BINDIR)

install:
	cp $(BINDIR)/$(TARGET) $(USR_LIB)
	cd $(USR_LIB); ln -s $(TARGET) $(SYMBOLIC_TARGET)

debug:
	$(MAKE) USR_CFLAGS=-g

clean:
	$(RM) $(objs) $(BINDIR)/$(TARGET)
	unlink $(USR_LIB)/$(SYMBOLIC_TARGET)
	$(RM) $(USR_LIB)/$(TARGET)

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@

%.o: $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@

