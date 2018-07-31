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
		vectors.S	\
		start.c	\
		

LOCAL_ALL=ejs.bin 

include $(ROOT_DIR)/Makefile.post

        
# only use LD, not work when use gcc
# if '-nostdlib' is not used, then first object should be defined in link script
$(LOCAL_ALL):$(OBJS)
		echo $(OBJS)
		$(LD) -nostdlib -T link.ld $(OBJS) -o ejs.elf
		$(OBJCOPY) -O binary ejs.elf $@
		$(MOVE) ejs.elf $(BIN_DIR)/sbin
		$(MOVE) $@ $(BIN_DIR)/sbin
