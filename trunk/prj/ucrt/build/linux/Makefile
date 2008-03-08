CC=gcc
TARGET=libucrt.so.1.0.0
SYMBOLIC_TARGET=libucrt.so.1
CFLAGS=-fPIC -c -Wall
LFLAGS=-shared -Wl,-soname,$(SYMBOLIC_TARGET) -o $(TARGET)
RM=rm -f
TARGET_INCDIR=../../../../inc/ucrt
INCDIR=../../inc
SRCDIR=../../src/os_posix
INCS=-I$(INCDIR)
BINDIR=../../../../bin
USR_LIB=/usr/lib
objs=ucrt_std.o\
	ucrt_file.o\
	ucrt_mem_file.o\
	ucrt_stdin.o\
	ucrt_stdout.o\
	ucrt_stderr.o\
	ucrt.o

install: ucrt
	sudo cp $(BINDIR)/$(TARGET) $(USR_LIB)
	cd $(USR_LIB); sudo ln -s $(TARGET) $(SYMBOLIC_TARGET)

ucrt: $(objs)
	$(CC) $(LFLAGS) $?
	cp $(INCDIR)/*.h $(TARGET_INCDIR)
	mv $(TARGET) $(BINDIR)
clean:
	$(RM) $(objs) $(BINDIR)/$(TARGET)
	$(RM) $(TARGET_INCDIR)/*
	sudo unlink $(USR_LIB)/$(SYMBOLIC_TARGET)
	sudo $(RM) $(USR_LIB)/$(TARGET)

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCS) $< -o $@

%.o: $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) $(INCS) $< -o $@

