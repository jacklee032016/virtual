#
# $Id: Makefile,v 1.1.1.1 2006/11/29 18:13:58 lizhijie Exp $
#


include $(RULE_DIR)/Makefile.pre

EXE=raspFree

CODE_SUBDIRS += \
			Source/portable/MemMang	\
			Source/portable/GCC/RaspberryPi \
			Demo/Drivers \
			Drivers/lan9514/lib \
			Drivers/tcp/portable/BufferManagement/ 


SOURCE_HEADER=-ISource/include/ 
PORTABLE_HEADER=-ISource/portable/GCC/RaspberryPi/ 
COMMON_HEADER=-IDrivers/ 
IP_HEADER=-IDrivers/tcp/include -IDrivers/lan9514/include 

#-IDrivers/lan9514/include/uspi  


CFLAGS += -Wall -Wextra $(SOURCE_HEADER) $(PORTABLE_HEADER) $(COMMON_HEADER) \
		$(IP_HEADER) \
		$(RASP_FLAGS) -DRASPPI=$(RASPPI) -Wno-psabi -fsigned-char -DRASPPI=$(RASPPI) -nostdlib -Wno-implicit -mfloat-abi=softfp \
		 -mno-unaligned-access \
		-fpic -ffreestanding -std=c99 

#		-g3 \
#		-gdwarf-2 \
#		-DPRINTF_FLOAT_SUPPORT \
#		-march=armv6z \
#-Werror 

# ASM_FLAGS= -march=armv6z
ASM_FLAGS= $(RASP_FLAGS)


DRIVER_SRC += \
		Drivers/interrupts.c \
		Drivers/gpio.c	\
		Drivers/video.c \

#things
DRIVER_SRC += \
		Drivers/mailbox.c \
		Drivers/mem.c \

#video stuff

#smsc9514 (LAN and USB)
DRIVER_LAN_USB += \
		Drivers/lan9514/uspibind.c \
		Drivers/lan9514/lib/uspilibrary.c \
		Drivers/lan9514/lib/dwhcidevice.c \
		Drivers/lan9514/lib/dwhciregister.c \
		Drivers/lan9514/lib/dwhcixferstagedata.c \
		Drivers/lan9514/lib/usbconfigparser.c \
		Drivers/lan9514/lib/usbdevice.c \
		Drivers/lan9514/lib/usbdevicefactory.c \
		Drivers/lan9514/lib/usbendpoint.c \
		Drivers/lan9514/lib/usbrequest.c \
		Drivers/lan9514/lib/usbstandardhub.c \
		Drivers/lan9514/lib/devicenameservice.c \
		Drivers/lan9514/lib/macaddress.c \
		Drivers/lan9514/lib/smsc951x.c \
		Drivers/lan9514/lib/string.c \
		Drivers/lan9514/lib/util.c \
		Drivers/lan9514/lib/usbmassdevice.c \
		Drivers/lan9514/lib/dwhciframeschednper.c \
		Drivers/lan9514/lib/dwhciframeschedper.c \
		Drivers/lan9514/lib/keymap.c \
		Drivers/lan9514/lib/usbkeyboard.c \
		Drivers/lan9514/lib/dwhcirootport.c \
		Drivers/lan9514/lib/usbmouse.c \
		Drivers/lan9514/lib/dwhciframeschednsplit.c \
		Drivers/lan9514/lib/usbgamepad.c \
		Drivers/lan9514/lib/synchronize.c \
		Drivers/lan9514/lib/usbstring.c \

#freeRTOS-TCP
DRIVER_TCP += \
		Drivers/tcp/FreeRTOS_ARP.c \
		Drivers/tcp/FreeRTOS_DHCP.c \
		Drivers/tcp/FreeRTOS_DNS.c \
		Drivers/tcp/FreeRTOS_IP.c \
		Drivers/tcp/FreeRTOS_Sockets.c \
		Drivers/tcp/FreeRTOS_Stream_Buffer.c \
		Drivers/tcp/FreeRTOS_TCP_IP.c \
		Drivers/tcp/FreeRTOS_TCP_WIN.c \
		Drivers/tcp/FreeRTOS_UDP_IP.c \
		Drivers/tcp/portable/BufferManagement/BufferAllocation_2.c \
		Drivers/tcp/portable/NetworkInterface.c \
		

DEMO_SRC += \
		Demo/trace.c \
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
		Source/event_groups.c \

	

SRC += \
		$(KERNEL_SRC) \
		$(PORTABLE_SRC) \
		$(DRIVER_SRC) \
		$(DRIVER_LAN_USB)	\
		$(DRIVER_TCP)	\
		$(DEMO_SRC) \
		

LOCAL_ALL=$(EXE).bin 

include $(ROOT_DIR)/Makefile.post


# dependend on ANSI lib form gcc; no start file is needed
# when using LD, memcpy, strlen and gcc lib can't be found. JL.Dec.31,2017
$(LOCAL_ALL):$(OBJS)
		$(CC) -nostartfiles -T raspberrypi.ld $(OBJS) -o $(EXE).elf -lm
		$(OBJCOPY) -O binary $(EXE).elf $@
		$(MOVE) $(EXE).elf $(BIN_DIR)/sbin
		$(MOVE) $@ $(BIN_DIR)/sbin
