CPP=g++
TARGET=libelfio.so.1.0.0
SYMBOLIC_TARGET=libelfio.so.1
CFLAGS=-fPIC -c -Wall
USR_CFLAGS=
LFLAGS=-shared -Wl,-soname,$(SYMBOLIC_TARGET) -o $(TARGET)
RM=rm -f
TARGET_INCDIR=../../../../inc/elfio
INCDIR=../../inc
SRCDIR=../../src
INCS=-I$(INCDIR) -I../../../../inc
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
	cp $(INCDIR)/elfio/*.h $(TARGET_INCDIR)
	mv $(TARGET) $(BINDIR)

install: elfio
	cp $(BINDIR)/$(TARGET) $(USR_LIB)
	cd $(USR_LIB); ln -s $(TARGET) $(SYMBOLIC_TARGET)

debug:
	$(MAKE) USR_CFLAGS=-g

clean:
	$(RM) $(objs) $(BINDIR)/$(TARGET)
	$(RM) $(TARGET_INCDIR)/*
	unlink $(USR_LIB)/$(SYMBOLIC_TARGET)
	$(RM) $(USR_LIB)/$(TARGET)

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@

%.o: $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@

