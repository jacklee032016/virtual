		Simplest Bare metal program
										JL(Jack Lee)

Flows:
	Reset Handler:
		The program/code executed when power on or reset CPU:
			linked as entry point(code, vectors_start) of all code: first instruct in vectors.obj is jump to reset handler;
			burned into position of flash or memory where CPU load first code;

		Flow:
			Setup stack: so function can be called;
			copy 8 handlers array (copy code) to address 0, which is default position for handlers of some hardware execeptions;
			Setup CSPR to enable IRQ;
			Enable URAT IRQ in IRQ register;
			Set stack for ISR: also just set SP register???;
			call or jump to main;
			
	Main:
		run forever;
		
	URAT ISR:
		reply char from UART with CHAR+1;

Link Script:
	define: Address of first instruction:
			. = 0x10000; this address is QEMU specified, not hardware, it also the startup address in ELF executive;
			
	stack_top and irq_stack_top are symbols defined in link script:
		symbols in link script can also be refered by assemble code or C code;
			in assemble code od vectors.S even without declarations before using it;
			
