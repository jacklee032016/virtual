ENTRY(vectors_start)
SECTIONS
{
 . = 0x10000;
 .text : {
/* Linux.obj.arm/vectors.o : define first object file directly, so '-nostdlib' is not used in Makefile. JL */
 *(.text .rodata)
 }
 .data : { *(.data) }
 .bss : { *(.bss) }
 . = ALIGN(8);
 . = . + 0x1000; /* 4kB of stack memory */
 stack_top = .;
 . = . + 0x1000; /* 4kB of irq stack memory */
 irq_stack_top = .;
}
