#
# $Id: Makefile,v 1.1.1.1 2006/11/29 18:13:58 lizhijie Exp $
#


include $(RULE_DIR)/Makefile.pre


CODE_SUBDIRS += \
			Source/portable/MemMang	\
			Source/portable/GCC/CortexA9 \
			Common/Minimal \
			CortexA9	\


SOURCE_HEADER=-ISource/include/ 
PORTABLE_HEADER=-ISource/portable/GCC/CortexA9/ 
COMMON_HEADER=-ICommon/include/ 
# for FreeRTOS configuration
DEMO_HEADER=-ICortexA9/ 

CFLAGS = -Wall -Wextra $(SOURCE_HEADER) $(PORTABLE_HEADER) $(COMMON_HEADER) $(DEMO_HEADER) \
		-fmessage-length=0 \
		-mcpu=cortex-a9 \
		-g3 \
		-gdwarf-2 \
		-std=c99 -DPRINTF_FLOAT_SUPPORT

#-Werror 

ASM_FLAGS= -mcpu=cortex-a9
#arm926ej-s 
################################################
####   source file 
################################################


COMMON_SRC += \
		Common/Minimal/BlockQ.c \
		Common/Minimal/blocktim.c \
		Common/Minimal/comtest.c \
		Common/Minimal/countsem.c \
		Common/Minimal/dynamic.c \
		Common/Minimal/flop.c \
		Common/Minimal/GenQTest.c \
		Common/Minimal/integer.c \
		Common/Minimal/PollQ.c \
		Common/Minimal/QPeek.c \
		Common/Minimal/recmutex.c \
		Common/Minimal/semtest.c


DEMO_SRC += \
		CortexA9/main.c \
		CortexA9/pl011.c \
		CortexA9/pl031_rtc.c \
		CortexA9/printf-stdarg.c \
		CortexA9/serial.c \
		CortexA9/sp804_timer.c \
		CortexA9/uart.c \
		CortexA9/startup.S

		
PORTABLE_SRC += \
		Source/portable/GCC/CortexA9/port.c \
		Source/portable/MemMang/heap_2.c \

KERNEL_SRC += \
		Source/croutine.c \
		Source/list.c \
		Source/queue.c \
		Source/tasks.c \
		Source/timers.c \
		
		

SRC += \
		$(KERNEL_SRC) \
		$(PORTABLE_SRC) \
		$(COMMON_SRC) \
		$(DEMO_SRC) \
		

LOCAL_ALL=pbxA9.bin 

include $(ROOT_DIR)/Makefile.post


# dependend on ANSI lib form gcc; no start file is needed
# when using LD, memcpy, strlen and gcc lib can't be found. JL.Dec.31,2017
# use gcc -mcpu=cortex-a9
$(LOCAL_ALL):$(OBJS)
		$(CC) -nostartfiles -T plain.ld $(OBJS) -o pbxA9.elf -lm
		$(OBJCOPY) -O binary pbxA9.elf $@
		$(MOVE) pbxA9.elf $(BIN_DIR)/sbin
		$(MOVE) $@ $(BIN_DIR)/sbin
