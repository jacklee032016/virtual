-M vexpress-a9 -serial stdio  -nographic -m 128 -kernel-serial stdio  
../../../buildQemu/arm-softmmu/qemu-system-arm -M vexpress-a9 -nographic -m 128 -kernel FreeRTOSDemo.0.elf 

memmap is different for xexpress-a9 and realview-pbx-a9:

Dec,28, 2017
	Debug and work for realview-pbx-a9:
			portPERIPHBASE in portmacros.h
			TIMER_0_1_BASE in sp_804.c( timer)
	
	Hardware : REALVIEW refer qemu/hw/arm/realview.c; vexpress : vexpress.c

Dec.27th, 2017
	qemu-system-arm -M realview-pbx-a9 -nographic -kernel FreeRTOSDemo.elf
	
	-A: arch  -O: os -T: type -C: compress -d: from datafile -n: set name of image
	Note: mkimage only run in Linux filesystem, because it call mmap()
	mkimage -A arm -O linux -T kernel -C none -a 0x10000 -e 0x10000 -d FreeRTOSDemo.bin -n FreeRTOS FreeRTOS_Demo.uimg

	qemu-system-arm -M realview-pbx-a9 -nographic -kernel u-boot-pb11mp.axf
	
	qemu-system-arm -M realview-pbx-a9 -nographic -kernel u-boot-pb11mp.axf -net nic -net user,tftp=".",bootfile="FreeRTOS_Demo.uimg" -s -S
	


Dec.25, 2017
	Build with arm-gcc49-linux-gnueabi-gcc:
		Target: arm-gcc49-linux-gnueabi
		error: required section '.rel.plt' not found in the linker script
		
	arm-none-eabi-gcc:
		Target: arm-none-eabi
		Thread model: single
		gcc version 4.8.1 
		
		