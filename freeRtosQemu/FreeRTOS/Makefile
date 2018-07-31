#
# $Id: Makefile,v 1.1.1.1 2006/11/29 18:13:58 lizhijie Exp $
#


include $(RULE_DIR)/Makefile.pre


CODE_SUBDIRS += \
			Source/portable/MemMang	\
			Source/portable/GCC/ARM926EJ-S \
			drivers \
			Demo	\


SOURCE_HEADER=-ISource/include/ 
PORTABLE_HEADER=-ISource/portable/GCC/ARM926EJ-S/ 
DRIVER_HEADER=-Idrivers/include/ 
DEMO_HEADER=-IDemo/ 

CFLAGS = -mcpu=arm926ej-s -Wall -Wextra -Werror $(SOURCE_HEADER) $(PORTABLE_HEADER) $(DRIVER_HEADER) $(DEMO_HEADER)

ASM_FLAGS= -mcpu=arm926ej-s 
################################################
####   source file 
################################################


PORTABLE_SRC += \
		Source/portable/MemMang/heap_1.c \
		Source/portable/GCC/ARM926EJ-S/port.c \
		Source/portable/GCC/ARM926EJ-S/portISR.c \

DRIVER_SRC += \
		drivers/timer.c	\
		drivers/interrupt.c \
		drivers/uart.c \


DEMO_SRC += \
		Demo/startup.s \
		Demo/init.c \
		Demo/print.c \
		Demo/receive.c \
		Demo/nostdlib.c \
		Demo/main.c \
		

		

SRC += \
			$(PORTABLE_SRC) \
			$(DRIVER_SRC) \
			$(DEMO_SRC) \
			Source/queue.c	\
			Source/list.c	\
			Source/tasks.c	\
		

LOCAL_ALL=rtos.bin 

include $(ROOT_DIR)/Makefile.post

          
# only use LD, not work when use gcc
$(LOCAL_ALL):$(OBJS)
		$(LD) -nostdlib -T qemu.ld $(OBJS) -o rtos.elf
		$(OBJCOPY) -O binary rtos.elf $@
		$(MOVE) rtos.elf $(BIN_DIR)/sbin
		$(MOVE) $@ $(BIN_DIR)/sbin
