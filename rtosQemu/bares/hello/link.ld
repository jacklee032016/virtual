
ENTRY(_Start)
SECTIONS
{
		. = 0x10000;
/*		startup : { ./Linux.obj.arm/startup.o(.text)}*/
		.text : {*(.text .rodata)}
		.data : {*(.data)}
		.bss : {*(.bss)}
		 . = ALIGN(8);
		 . = . + 0x500; /* stack memory */
		sp_top = .; /* stack pointer */
}
