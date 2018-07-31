# $Id$

RASPPI:=2

QEMU=YES

ROOT_DIR:=$(shell pwd)
RULE_DIR:=$(ROOT_DIR)
#/../

ARCH=arm

export RASPPI
export QEMU

export ROOT_DIR
export RULE_DIR
EXTENSION=

#BUILDTIME := $(shell TZ=UTC date -u "+%Y_%m_%d-%H_%M")
BUILDTIME := $(shell TZ=CN date -u "+%Y_%m_%d")
GCC_VERSION := $(shell $(CC) -dumpversion )


RELEASES_NAME=projects_$(GCC_VERSION)_$(ARCH)_$(EDITION)_$(BUILDTIME).tar.gz  


export ARCH
export BUILDTIME
export RELEASES_NAME
export CFLAGS



SUBDIRS += first
#SUBDIRS += raspMini
SUBDIRS += full


all: 
	for i in $(SUBDIRS) ; do ( cd $$i && $(MAKE) $@ ) ; done

clean: 
	rm -rf projects_*
	rm -rf pkg
	rm -rf Linux.bin.* 
	rm -rf *.log
	- find . -name Linux.obj.* -prune -exec rm -r -f {} \;
	
# all	
install:
	@$(SHELL) $(ROOT_DIR)/buildver.sh $(ROOT_DIR)
	@$(SHELL) $(ROOT_DIR)/install.sh $(ROOT_DIR)/Linux.bin.$(ARCH) $(ROOT_DIR)/releases  

#	@$(SHELL) $(ROOT_DIR)/buildver.sh $(ROOT_DIR)

package:clean
	cd ..; tar -cvjf rasp.$(BUILDTIME).tar.bz2 projects

