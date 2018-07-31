
Jan.8th, 2018
	Update FreeRTOS for EJ-S to 10.0.0;


	bares: bare metal without OS
			hello: for ejs, ouput 'Hello Open World';
					qemu-system-arm -M versatilepb -nographic -kernel ./Linux.bin.arm/sbin/hello.elf|bin

			uartReply: for 926EJ-S; reply the char of char+1;
					qemu-system-arm -M versatilepb -nographic -kernel ./Linux.bin.arm/sbin/uartReply.elf|bin
					
	ejs: FreeRTOS tasks for 926EJ-S;
					qemu-system-arm -M versatilepb -nographic -kernel ./Linux.bin.arm/sbin/ejs.elf|bin
	
	cortexA9: FreeRTOS for Cortex-A9
					qemu-system-arm -M realview-pbx-a9 -nographic -kernel ./Linux.bin.arm/sbin/pbxA9.elf|bin		

					