#
# $Id: Makefile,v 1.1.1.1 2006/11/29 18:13:58 lizhijie Exp $
#


include $(RULE_DIR)/Makefile.pre


CODE_SUBDIRS += \
			.


CFLAGS = -mcpu=arm926ej-s -Wall -Wextra -Werror 

ASM_FLAGS= -mcpu=arm926ej-s 
################################################
####   source file 
################################################
		

SRC += \
		startup.s	\
		hello.c	\
		

LOCAL_ALL=hello.bin 

include $(ROOT_DIR)/Makefile.post

        
# only use LD, not work when use gcc
# if '-nostdlib' is not used, then first object should be defined in link script
$(LOCAL_ALL):$(OBJS)
		echo $(OBJS)
		$(LD) -nostdlib -T link.ld $(OBJS) -o hello.elf
		$(OBJCOPY) -O binary hello.elf $@
		$(MOVE) hello.elf $(BIN_DIR)/sbin
		$(MOVE) $@ $(BIN_DIR)/sbin
