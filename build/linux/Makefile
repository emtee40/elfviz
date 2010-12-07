include config.min
TOPDIR=../..
BINDIR=$(TOPDIR)/bin

all:
	cd $(TOPDIR)/prj/elfio/build/linux && make all 
	cd $(TOPDIR)/prj/elfviz/build/linux && make all 
	cd $(TOPDIR)/prj/elf2txt/build/linux && make all 
	cd $(TOPDIR)/prj/gelfviz/build/linux && make all 

debug:
	cd $(TOPDIR)/prj/elfio/build/linux && make USR_CFLAGS=-g 
	cd $(TOPDIR)/prj/elfviz/build/linux && make USR_CFLAGS=-g 
	cd $(TOPDIR)/prj/elf2txt/build/linux && make USR_CFLAGS=-g 
	cd $(TOPDIR)/prj/gelfviz/build/linux && make USR_CFLAGS=-g 

clean:
	cd $(TOPDIR)/prj/elfio/build/linux && make clean 
	cd $(TOPDIR)/prj/elfio/build/linux && make clean  DIST_PREFIX="../../../../bin/debian"
	cd $(TOPDIR)/prj/elfviz/build/linux && make clean
	cd $(TOPDIR)/prj/elf2txt/build/linux && make clean
	cd $(TOPDIR)/prj/gelfviz/build/linux && make clean

install:
	cd $(TOPDIR)/prj/elfio/build/linux && make install 
	cd $(TOPDIR)/prj/elfviz/build/linux && make install
	cd $(TOPDIR)/prj/elf2txt/build/linux && make install
	cd $(TOPDIR)/prj/gelfviz/build/linux && make install

dist: 
	cd $(TOPDIR)/prj/elfio/build/linux && make install  DIST_PREFIX="../../../../bin/debian"
	cd $(TOPDIR)/prj/elfviz/build/linux && make install DIST_PREFIX="../../../../bin/debian"
	cd $(TOPDIR)/prj/elf2txt/build/linux && make install DIST_PREFIX="../../../../bin/debian"
	cd $(TOPDIR)/prj/gelfviz/build/linux && make install DIST_PREFIX="../../../../bin/debian"
	cp /usr/lib/libucrt-2.0.0.so /usr/lib/libucxml-2.0.0.so ../../bin/debian/usr/lib

src_tar:
	cd $(TOPDIR) && tar -zcvf $(EV_TITLE)-src.tar.gz * --exclude ".svn"

