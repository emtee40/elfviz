include config.min
TOPDIR=../..
BINDIR=$(TOPDIR)/bin

all:
	cd $(TOPDIR)/prj/elfio/build/linux && make all TARGET=$(EV_TARGET)
	cd $(TOPDIR)/prj/elfviz/build/linux && make all TARGET=$(EV_TARGET)
	cd $(TOPDIR)/prj/elf2txt/build/linux && make all TARGET=$(EV_TARGET)
	cd $(TOPDIR)/prj/gelfviz/build/linux && make all TARGET=$(EV_TARGET)

debug:
	cd $(TOPDIR)/prj/elfio/build/linux && make USR_CFLAGS=-g TARGET=$(EV_TARGET)
	cd $(TOPDIR)/prj/elfviz/build/linux && make USR_CFLAGS=-g TARGET=$(EV_TARGET)
	cd $(TOPDIR)/prj/elf2txt/build/linux && make USR_CFLAGS=-g TARGET=$(EV_TARGET)
	cd $(TOPDIR)/prj/gelfviz/build/linux && make USR_CFLAGS=-g TARGET=$(EV_TARGET)

clean:
	cd $(TOPDIR)/prj/elfio/build/linux && make clean TARGET=$(EV_TARGET)
	cd $(TOPDIR)/prj/elfio/build/linux && make clean TARGET=$(EV_TARGET) DIST_PREFIX="../../../../bin/debian"
	cd $(TOPDIR)/prj/elfviz/build/linux && make clean
	cd $(TOPDIR)/prj/elf2txt/build/linux && make clean
	cd $(TOPDIR)/prj/gelfviz/build/linux && make clean

install:
	cd $(TOPDIR)/prj/elfio/build/linux && make install TARGET=$(EV_TARGET)
	cd $(TOPDIR)/prj/elfviz/build/linux && make install
	cd $(TOPDIR)/prj/elf2txt/build/linux && make install
	cd $(TOPDIR)/prj/gelfviz/build/linux && make install

dist: 
	cd $(TOPDIR)/prj/elfio/build/linux && make install TARGET=$(EV_TARGET) DIST_PREFIX="../../../../bin/debian"
	cd $(TOPDIR)/prj/elfviz/build/linux && make install DIST_PREFIX="../../../../bin/debian"
	cd $(TOPDIR)/prj/elf2txt/build/linux && make install DIST_PREFIX="../../../../bin/debian"
	cd $(TOPDIR)/prj/gelfviz/build/linux && make install DIST_PREFIX="../../../../bin/debian"

src_tar:
	cd $(TOPDIR) && tar -zcvf $(EV_TITLE)-src.tar.gz * --exclude ".svn"

