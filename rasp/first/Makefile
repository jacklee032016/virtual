#
#  $
#


include $(RULE_DIR)/Makefile.pre

EXE=first

CODE_SUBDIRS += \
			Source/portable/MemMang	\
			Source/portable/GCC/RaspberryPi \
			Demo/Drivers \


SOURCE_HEADER=-ISource/include/ 
PORTABLE_HEADER=-ISource/portable/GCC/RaspberryPi/ 
COMMON_HEADER=-IDemo/Drivers/ 
# for FreeRTOS configuration
DEMO_HEADER=-IDemo/ 

CFLAGS += -Wall -Wextra $(SOURCE_HEADER) $(PORTABLE_HEADER) $(COMMON_HEADER) $(DEMO_HEADER) \
		$(RASP_FLAGS) -fpic -ffreestanding

ASM_FLAGS= 


DRIVER_SRC += \
		Demo/Drivers/irq.c \
		Demo/Drivers/gpio.c


DEMO_SRC += \
		Demo/main.c	\
		Demo/startup.S	\

		
PORTABLE_SRC += \
		Source/portable/GCC/RaspberryPi/port.c \
		Source/portable/GCC/RaspberryPi/portisr.c \
		Source/portable/MemMang/heap_4.c \


KERNEL_SRC += \
		Source/croutine.c \
		Source/list.c \
		Source/queue.c \
		Source/tasks.c \

		

SRC += \
		$(KERNEL_SRC) \
		$(PORTABLE_SRC) \
		$(DRIVER_SRC) \
		$(DEMO_SRC) \
		

LOCAL_ALL=$(EXE).bin 

include $(ROOT_DIR)/Makefile.post


ifeq ($(QEMU),YES)
	LINK_SCRIPT=raspQemu.ld
else
	LINK_SCRIPT=raspberrypi.ld
endif

# dependend on ANSI lib form gcc; no start file is needed -lm -nostdlib 
# when using LD, memcpy, strlen and gcc lib can't be found. JL.Dec.31,2017
$(LOCAL_ALL):$(OBJS)
		$(CC) -nostartfiles $(RASP_FLAGS)  -T $(LINK_SCRIPT) $(OBJS) -o $(EXE).elf 
		$(OBJCOPY) -O binary $(EXE).elf $@
		$(MOVE) $(EXE).elf $(BIN_DIR)/sbin
		$(MOVE) $@ $(BIN_DIR)/sbin
