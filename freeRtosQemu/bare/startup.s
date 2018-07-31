.global _Start
_Start:
LDR sp, = sp_top /* load Stack Pointer (sp) which is defined in linker script */
BL my_init
B .

