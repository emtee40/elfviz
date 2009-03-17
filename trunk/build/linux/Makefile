TOPDIR=../..
BINDIR=$(TOPDIR)/bin

INSTBIN=/usr/bin
INSTLIB=/usr/lib
INST_SCRIPT=install.sh
UNINST_SCRIPT=uninstall.sh

all:
	cd $(TOPDIR)/prj/elfio/build/linux && make all
	cd $(TOPDIR)/prj/elfviz/build/linux && make all
	cd $(TOPDIR)/prj/elf2txt/build/linux && make all
	cd $(TOPDIR)/prj/gelfviz/build/linux && make all
	cp $(INST_SCRIPT) $(UNINST_SCRIPT) $(BINDIR)

debug:
	cd $(TOPDIR)/prj/elfio/build/linux && make USR_CFLAGS=-g
	cd $(TOPDIR)/prj/elfviz/build/linux && make USR_CFLAGS=-g
	cd $(TOPDIR)/prj/elf2txt/build/linux && make USR_CFLAGS=-g
	cd $(TOPDIR)/prj/gelfviz/build/linux && make USR_CFLAGS=-g
	cp $(INST_SCRIPT) $(UNINST_SCRIPT) $(BINDIR)

clean:
	cd $(TOPDIR)/prj/elfio/build/linux && make clean
	cd $(TOPDIR)/prj/elfviz/build/linux && make clean
	cd $(TOPDIR)/prj/elf2txt/build/linux && make clean
	cd $(TOPDIR)/prj/gelfviz/build/linux && make clean
	rm -f $(BINDIR)/$(INST_SCRIPT) $(BINDIR)/$(UNINST_SCRIPT)

install:
	$(BINDIR)/install.sh

src_tar:
	cd $(TOPDIR) && tar -zcvf elfviz-2.0.0-src.tar.gz * --exclude ".svn"

tar:
	cd $(BINDIR) && tar -zcvf elfviz-2.0.0-i386-linux.tar.gz * --exclude ".svn"

dist_clean: clean
	./uninstall.sh

