#
# $
#


include $(RULE_DIR)/Makefile.pre

EXE=mini
CODE_SUBDIRS += \
			.


CFLAGS += $(RASP_FLAGS) -fpic -ffreestanding 
 
ASM_FLAGS= 
################################################
####   source file 
################################################


SRC += \
	main.c	\
	startup.S	\
		

LOCAL_ALL=$(EXE).bin 

include $(ROOT_DIR)/Makefile.post
          
# only use LD, not work when use gcc
$(LOCAL_ALL):$(OBJS)
		$(LD) -nostdlib -T rasp.ld $(OBJS) -o $(EXE).elf
		$(OBJCOPY) -O binary $(EXE).elf $@
		$(MOVE) $(EXE).elf $(BIN_DIR)/sbin
		$(MOVE) $@ $(BIN_DIR)/sbin
