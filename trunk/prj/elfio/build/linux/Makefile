MAKE=make
CPP=g++
AR=ar rcu
CFLAGS=-c
USR_CFLAGS=
RM=rm -f
INCDIR=../../inc
SRCDIR=../../src
TARGET_INCDIR=../../../../inc
INCS=-I$(SRCDIR) -I$(INCDIR) -I$(TARGET_INCDIR)
LIBDIR=../../../../lib
objs=elfio.o \
	phdr.o \
	phdr_entry.o \
	shdr.o \
	shdr_entry.o \
	shdr_symtab_entry.o
OUT=libelfio.a

tm: $(objs)
	$(AR) $(OUT) $?
	cp $(INCDIR)/elfio/*.h ../../../../inc/elfio
	mv $(OUT) $(LIBDIR)
debug:
	$(MAKE) USR_CFLAGS=-g
clean:
	$(RM) $(objs) $(LIBDIR)/$(OUT)
	$(RM) $(TARGET_INCDIR)

%.o: $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) $(USR_CFLAGS) $(INCS) $< -o $@
