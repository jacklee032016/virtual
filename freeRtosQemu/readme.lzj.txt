

Dec.25th,2018
		Build with 'as': option '--MD file': out dependency info into file;
		
		Link with '-nostdlib' : disable all options standard C app, such as 'crt1.o' (not just main entry point )
		Or define first object in link scrips;
		

Dec.24th, 2017, Sunday
		*.s : build with 'as';
		*.S : build with 'gcc';
		
		link sccript defines obj file and the position where it is linked;
		
		Delete qemu thread:
				ps -a | awk ' /qemu-system-arm/ {print $1} '
				

Dec.22ns, 2017, Friday
Toolchain:
		GCC ARM EABI toolchain

Run: work both with elf or bin formats
../buildQemu/arm-softmmu/qemu-system-arm -M versatilepb -nographic -m 128 -kernel ./Linux.bin.arm/sbin/rtos.bin


 -net user -net nic,model=smc91c111
 
exit qemu:
		Ctrl+A --> C
		
qemu-system-arm -M ? : list all machine


../buildQemu/arm-softmmu/qemu-system-arm -M xilinx-zynq-a9 -kernel ../u-boot-xlnx-master/u-boot.bin -serial null -serial mon:stdio -nographic -m 256
 