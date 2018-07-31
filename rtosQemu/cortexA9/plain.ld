ENTRY(Vector_Init); /* Vector_Init is first address in isr_vector in startup.S */

MEMORY
{
	ram (rwx) : ORIGIN = 0x10000, LENGTH = ( 64M - 0x10000 )
	//flash (rx) : ORIGIN = 0x10000, LENGTH = ( 64M - 0x10000 )
	//ram (rwx) : ORIGIN = 0x4001000, LENGTH = ( 128M - 0x1000 )
	//sdram (rwx) : ORIGIN = 0x20000000, LENGTH = 512M
}

PROVIDE(__stack = 0x10000);

SECTIONS
{
    .text :
    {
        _text = .;	/* symbol of first address of text which is used by other core in MPCore */
        __isr_vector_start = .;	
        *(.isr_vector)	/* this is a section defined in startup.S */
        __isr_vector_end = .;
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > ram
    .data : /*AT(ADDR(.text) + SIZEOF(.text))*/
    {
        _data = .;
        *(vtable)
        *(.data*)
        _edata = .;
    } > ram
    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > ram
}

