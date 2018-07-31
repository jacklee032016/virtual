

Raspberry Pi 2:
		make rpi_2_config : in arch/../configs/rpi_2_defconfig
		make all CROSS_COMPILE=arm-gcc49-linux-gnueabi- V=1 > mk.log
		make all CROSS_COMPILE=arm-none-eabi- V=1 > mk.log

qemu-system-arm -M raspi2 -kernel u-boot
		see output in serial console in qemu monitor

Framebuffer is not initialized;
ln: failed to create symbolic link 'arch/arm/include/asm/arch': Read-only file system


Jan.6th, 
	About build:
		About float:
			when '-mfloat-abi=hard' is used in, link fails becuase libc.a use VFP register;
			Because this program calls memcpy etc standard libraries;
		
		'-nostartfiles': no crn_it and other standrad start module for C program; can be used in both build and link;
		'-nostdlib': no link to standard library from toolchain;
		

	Hardware of Ras Pi2 B:
		ARMv7, Corext-A7 (For Pi 1, ARMv6, arm1176, which support MPCore)
	
	Apps:
		mini: 
			Only 2 parts of code: 
				Stack and bss setup, the jump to main: regular ARM asm, can run on any ARM cpu;
				UART operation and show 'hello world': RaspBerry external device, can run RasPi1 and RasPi2.
			Build: 
				Pi-1:		arm1176jzf-s, -mcpu=arm1176jzf-s or -mtune=arm1176jzf-s
				Pi-2:		-mcpu|mtune=cortex-a7 -mfloat-abi=hard
				Note: gcc --target-help for build options about platform;
			Run:
				qemu-system-arm -M raspi2 -nographic -kernel Linux.bin.arm/sbin/mini.elf 
				not working:
				qemu-system-arm -M versatilepb -cpu arm1176 -nographic -kernel Linux.bin.arm/sbin/mini.elf	


Test qemu-rpi-kernel
		qemu-system-arm -kernel ./kernel-qemu-4.4.34-jessie -cpu arm1176 -m 256 -M versatilepb -nographic
		qemu-system-arm -kernel ./kernel-qemu-4.4.34-jessie -cpu arm1176 -m 256 -M versatilepb -serial stdio 

	-nographic: no graphic window is open, all output to stdio will sent to current console;
			only show: Uncompressing Linux... done, booting the kernel.
	-serial stdio: open a new window as graphic output, then use stdio of new window as serial output;
			after show: Uncompressing Linux... done, booting the kernel, then a new graphic window with Raspbeery logo;

	-M versatilepb -cpu arm1176:
			arm1176 is arm11, eg.ARMv6
			versatilepb of qemu is ARM926EJ-S: arm9, eg.ARMv5
			How to compile it?

qemu-img convert -f raw -O qcow2 2017-11-29-raspbian-stretch-lite.img raspbian-stretch-lite.qcow  

qemu-system-arm -kernel ./kernel-qemu-4.4.34-jessie -cpu arm1176 -m 256 -M versatilepb -serial stdio 

	


qemu-system-arm \
	-kernel ./kernel-qemu-4.4.34-jessie \
	-append "root=/dev/sda2 panic=1 rootfstype=ext4 rw" \
	-hda raspbian-stretch-lite.qcow \
	-cpu arm1176 -m 256 \
	-M versatilepb \
	-no-reboot \
	-serial stdio \
	-net nic -net user \
	-net tap,ifname=vnet0,script=no,downscript=no


# Create an 1 GiB SD card image from the downloaded OSMC image.
$ cat ~/Downloads/OSMC_TGT_rbp2_20160130.img /dev/zero | dd bs=4096 count=262144 >/tmp/osmc.img
262144+0 Datens?tze ein
262144+0 Datens?tze aus
1073741824 Bytes (1,1 GB) kopiert, 3,15888 s, 340 MB/s

# Mount the first partition from the OSMC image.
$ sudo kpartx -a /home/stefan/Downloads/OSMC_TGT_rbp2_20160130.img
$ sudo mount /dev/mapper/loop0p1 /mnt -o loop,ro


Command line:

$ qemu-system-arm -M raspi -L pc-bios -kernel vmlinux --append "dma.dmachans=0x3c bcm2708_fb.fbwidth=1280 bcm2708_fb.fbheight=1024 bcm2708.boardrev=0x2 bcm2708.serial=0xe16a63c5 smsc95xx.macaddr=B8:27:EB:6A:63:C5 dwc_otg.lpm_enable=0 console=ttyAMA0,115200 kgdboc=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait debug initcall_debug"

File dmesg.log was created without debug information:

$ qemu-system-arm -M raspi -L pc-bios -kernel vmlinux --append "dma.dmachans=0x3c bcm2708_fb.fbwidth=1280 bcm2708_fb.fbheight=1024 bcm2708.boardrev=0x2 bcm2708.serial=0xe16a63c5 smsc95xx.macaddr=B8:27:EB:6A:63:C5 dwc_otg.lpm_enable=0 console=ttyAMA0,115200 kgdboc=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait"


 arm-softmmu/qemu-system-arm -M raspi2 -kernel /mnt/kernel.img -append "$(cat /mnt/cmdline.txt)" \
 -usbdevice mouse -usbdevice keyboard -drive file=/tmp/osmc.img,if=sd,format=raw


					