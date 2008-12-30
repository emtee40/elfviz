TOPDIR=../..
BINDIR=$(TOPDIR)/bin

INSTBIN=/usr/bin
INSTLIB=/usr/lib

all:
	cd $(TOPDIR)/prj/elfio/build/linux && make all
	cd $(TOPDIR)/prj/elfviz/build/linux && make all

debug:
	cd $(TOPDIR)/prj/elfio/build/linux && make USR_CFLAGS=-g
	cd $(TOPDIR)/prj/elfviz/build/linux && make USR_CFLAGS=-g

install:
	mv $(BINDIR)/libelfio* $(INSTLIB)
	mv $(BINDIR)/elfviz $(INSTBIN)

clean:
	cd $(TOPDIR)/prj/elfio/build/linux && make clean
	cd $(TOPDIR)/prj/elfviz/build/linux && make clean

dist_clean: clean
	rm $(INSTBIN)/elfviz $(INSTLIB)/libelfio*

