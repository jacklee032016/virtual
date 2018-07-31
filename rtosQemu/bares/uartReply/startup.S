/* IRQ vector in AEM926-EJS 
*/

.text
.code 32
 
 .global vectors_start
 .global vectors_end
 
vectors_start:/* first address as main defined in link script */
 /* address of reset_handler which is first instruction after reset*/
 LDR PC, reset_handler_addr		/* first instruct is jump to reset_handler */
 LDR PC, undef_handler_addr
 LDR PC, swi_handler_addr
 LDR PC, prefetch_abort_handler_addr
 LDR PC, data_abort_handler_addr
 B .
 LDR PC, irq_handler_addr
 LDR PC, fiq_handler_addr

/* data for 7 handlers' address; following only are macros of fetching address, not occpupied the memory */ 
reset_handler_addr: .word reset_handler
undef_handler_addr: .word undef_handler
swi_handler_addr: .word swi_handler
prefetch_abort_handler_addr: .word prefetch_abort_handler
data_abort_handler_addr: .word data_abort_handler
irq_handler_addr: .word irq_handler
fiq_handler_addr: .word fiq_handler

/* from vectors_start to vectors_stop, 8 words */ 
vectors_end:


/* reset handler: handler after power on */
reset_handler:
 /* set Supervisor stack */
 LDR sp, =stack_top	/* address of stack top defined in link script */
 
 /* copy vector table to address 0 */
 /* BL: call function, LDR PC: set PC; function must only be called after stack setup */
 BL copy_vectors
 
 /* get Program Status Register */
 MRS r0, cpsr
 /* go in IRQ mode */
 BIC r1, r0, #0x1F
 ORR r1, r1, #0x12
 MSR cpsr, r1
 
 /* set IRQ stack */
 /**/
 LDR sp, =irq_stack_top
 
 /* Enable IRQs */
 BIC r0, r0, #0x80
 
 /* go back in Supervisor mode */
 MSR cpsr, r0
 
 /* jump to main */
 BL main
 B .
 
.end
