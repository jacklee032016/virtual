
/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the ARM Cortex-A9 port.
 *----------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"


#include "cortexOs.h"


/* For backward compatibility, ensure configKERNEL_INTERRUPT_PRIORITY is
defined.  The value should also ensure backward compatibility.
FreeRTOS.org versions prior to V4.4.0 did not include this definition. */
#ifndef configKERNEL_INTERRUPT_PRIORITY
	#define configKERNEL_INTERRUPT_PRIORITY 255
#endif

/* Constants required to set up the initial stack. */
#define portINITIAL_XPSR			( 0x0000001F )

/* Interrupt Handler Support. */
typedef struct STRUCT_HANDLER_PARAMETER
{
	void (*vHandler)(void *);
	void *pvParameter;
} xInterruptHandlerDefinition;
xInterruptHandlerDefinition pxInterruptHandlers[ portMAX_VECTORS ] = { { NULL, NULL } };

extern unsigned portBASE_TYPE * volatile pxCurrentTCB;

/* Definition of the max vector id. */
static unsigned long ulMaxVectorId = portMAX_VECTORS;

/* The priority used by the kernel is assigned to a variable to make access
from inline assembler easier. */
const unsigned long ulKernelPriority = configKERNEL_INTERRUPT_PRIORITY;

/* Declare some stack space for each mode. */
static portSTACK_TYPE puxFIQStack[ portFIQ_STACK_SIZE ];
static portSTACK_TYPE puxIRQStack[ portIRQ_STACK_SIZE ];
static portSTACK_TYPE puxAbortStack[ portABORT_STACK_SIZE ];
static portSTACK_TYPE puxSVCStack[ portSVC_STACK_SIZE ];

static portSTACK_TYPE *puxFIQStackPointer = &(puxFIQStack[ portFIQ_STACK_SIZE - 1 ] );
static portSTACK_TYPE *puxIRQStackPointer = &(puxIRQStack[ portIRQ_STACK_SIZE - 1 ] );
static portSTACK_TYPE *puxAbortStackPointer = &(puxAbortStack[ portABORT_STACK_SIZE - 1 ] );
static portSTACK_TYPE *puxSVCStackPointer = &(puxSVCStack[ portSVC_STACK_SIZE - 1 ] );

/* Page table */
static unsigned long PageTable[4096] __attribute__((aligned (16384)));

/*
 * Setup the timer to generate the tick interrupts.
 */
static void prvSetupTimerInterrupt( void );

/*
 * Exception handlers.
 */
void vPortPendSVHandler( void *pvParameter ) __attribute__((naked));
void vPortSysTickHandler( void *pvParameter );
void vPortSVCHandler( void ) __attribute__ (( naked ));
void vPortInterruptContext( void ) __attribute__ (( naked ));
void vPortSMCHandler( void ) __attribute__ (( naked ));

/*
 * Start first task is a separate function so it can be tested in isolation.
 */
void vPortStartFirstTask( void ) __attribute__ (( naked ));

/*-----------------------------------------------------------*/

/* Linker defined variables. */
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;
extern unsigned long _stack_top;
/*----------------------------------------------------------------------------*/


void vPortInstallInterruptHandler( void (*vHandler)(void *), void *pvParameter, unsigned long ulVector, unsigned char ucEdgeTriggered, unsigned char ucPriority, unsigned char ucProcessorTargets )
{
	unsigned long ulBank32 = 4 * ( ulVector / 32 );
	unsigned long ulOffset32 = ulVector % 32;
	unsigned long ulBank4 = 4 * ( ulVector / 4 );
	unsigned long ulOffset4 = ulVector % 4;
	unsigned long ulBank16 = 4 * ( ulVector / 16 );
	unsigned long ulOffset16 = ulVector % 16;
	/* unsigned long puxGICAddress = 0; */
	unsigned long puxGICDistributorAddress = 0;

	/* Select which GIC to use. */
	if ( ulVector < 32 )
	{
		/* puxGICAddress = portGIC_PRIVATE_BASE; */
		puxGICDistributorAddress = portGIC_DISTRIBUTOR_BASE;
	}
	else
	{
		/* Shouldn't get here. */
	}

	/* Record the Handler. */
	if (ulVector < ulMaxVectorId )
	{
		pxInterruptHandlers[ ulVector ].vHandler = vHandler;
		pxInterruptHandlers[ ulVector ].pvParameter = pvParameter;

		/* Now calculate all of the offsets for the specific GIC. */
		ulBank32 = 4 * ( ulVector / 32 );
		ulOffset32 = ulVector % 32;
		ulBank4 = 4 * ( ulVector / 4 );
		ulOffset4 = ulVector % 4;
		ulBank16 = 4 * ( ulVector / 16 );
		ulOffset16 = ulVector % 16;

		/* Is it Edge Triggered?. */
		if ( 0 != ucEdgeTriggered )
		{
			portGIC_SET( (portGIC_ICDICR_BASE(puxGICDistributorAddress + ulBank16) ), ( portGIC_READ(puxGICDistributorAddress + ulBank16) | ( 0x02 << ( ulOffset16 * 2 ) ) ) );
		}

		/* Set the Priority. */
		portGIC_WRITE( portGIC_ICDIPR_BASE(puxGICDistributorAddress) + ulBank4, ( ( (unsigned long)ucPriority ) << ( ulOffset4 * 8 ) ) );

		/* Set the targeted Processors. */
		portGIC_WRITE( portGIC_ICDIPTR_BASE(puxGICDistributorAddress + ulBank4), ( ( (unsigned long)ucProcessorTargets ) << ( ulOffset4 * 8 ) ) );

		/* Enable the Interrupt. */
		if ( NULL != vHandler )
		{
			portGIC_SET( portGIC_ICDICPR_BASE(puxGICDistributorAddress + ulBank32), ( portGIC_READ( portGIC_ICDICPR_BASE(puxGICDistributorAddress + ulBank32) ) | ( 1 << ulOffset32 ) ) );
			portGIC_SET( portGIC_ICDISER_BASE(puxGICDistributorAddress + ulBank32), ( portGIC_READ( portGIC_ICDISER_BASE(puxGICDistributorAddress + ulBank32) ) | ( 1 << ulOffset32 ) ) );
		}
		else
		{
			/* Or disable when passed a NULL handler. */
			portGIC_CLEAR( portGIC_ICDICPR_BASE(puxGICDistributorAddress + ulBank32), ( portGIC_READ( portGIC_ICDICPR_BASE(puxGICDistributorAddress + ulBank32) ) | ( 1 << ulOffset32 ) ) );
			portGIC_CLEAR( portGIC_ICDISER_BASE(puxGICDistributorAddress + ulBank32), ( portGIC_READ( portGIC_ICDISER_BASE(puxGICDistributorAddress + ulBank32) ) | ( 1 << ulOffset32 ) ) );
		}
	}
}


/*
 * See header file for description.
 */
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{
	portSTACK_TYPE *pxOriginalStack = pxTopOfStack;
	/* Align top of stack */
	pxTopOfStack = (portSTACK_TYPE *)((unsigned long)(pxTopOfStack)&~portBYTE_ALIGNMENT_MASK);
	/* Simulate the stack frame as it would be created by a context switch interrupt. */
	*pxTopOfStack = portINITIAL_XPSR;	/* xPSR */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) pxCode;	/* PC */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0;	/* LR */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) pxOriginalStack;	/* SP */
	pxTopOfStack -= 12;		/* R1 through R12 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) pvParameters;	/* R0 */

	return pxTopOfStack;
}


/* SWI exception */
void vPortSVCHandler( void )
{
	__asm volatile(
			" ldr r9, pxCurrentTCBConst2	\n"		/* Load the pxCurrentTCB pointer address. */
			" ldr r8, [r9]					\n"		/* Load the pxCurrentTCB address. */
			" ldr lr, [r8]					\n"		/* Load the Task Stack Pointer into LR. */
			" ldmia lr, {r0-lr}^		 	\n"		/* Load the Task's registers. */
			" add lr, lr, #60			 	\n"		/* Re-adjust the stack for the Task Context */
			" nop						 	\n"
			" rfeia lr				 		\n"		/* Return from exception by loading the PC and CPSR from Task Stack. */
			" nop						 	\n"
			"								\n"
			"	.align 2					\n"
			" pxCurrentTCBConst2: .word pxCurrentTCB	\n"
			);
	xUARTSendCharacter(portCORE_ID(), 'S',0);

}


void vPortInterruptContext( void )
{
	__asm volatile(
			" sub lr, lr, #4				\n"		/* Adjust the return address. */
			" srsdb SP, #31					\n"		/* Store the return address and SPSR to the Task's stack. */
			" stmdb SP, {SP}^			 	\n"		/* Store the SP_USR to the stack. */
			" sub SP, SP, #4			 	\n"		/* Decrement the Stack Pointer. */
			" ldmia SP!, {lr}			 	\n"		/* Load the SP_USR into LR. */
			" sub LR, LR, #8 				\n"		/* Make room for the previously stored LR and CPSR. */
			" stmdb LR, {r0-lr}^	 		\n"		/* Store the Task's registers. */
			" sub LR, LR, #60		 		\n"		/* Adjust the Task's stack pointer. */
			" ldr r9, pxCurrentTCBConst2	\n"		/* Load the pxCurrentTCB pointer address. */
			" ldr r8, [r9]					\n"		/* Load the pxCurrentTCB address. */
			" str lr, [r8]	 				\n"		/* Store the Task stack pointer to the TCB. */
			" bl vPortGICInterruptHandler	\n"		/* Branch and link to find specific service handler. */
			" ldr r8, [r9]					\n"		/* Load the pxCurrentTCB address. */
			" ldr lr, [r8]					\n"		/* Load the Task Stack Pointer into LR. */
			" ldmia lr, {r0-lr}^		 	\n"		/* Load the Task's registers. */
			" add lr, lr, #60			 	\n"		/* Re-adjust the stack for the Task Context */
			" rfeia lr				 		\n"		/* Return from exception by loading the PC and CPSR from Task Stack. */
			" nop						 	\n"
			);
}


void vPortStartFirstTask( void )
{
	__asm volatile(
		" mov SP, %[svcsp]			\n" /* Set-up the supervisor stack. */
		" svc 0 					\n" /* Use the supervisor call to be in an exception. */
		" nop						\n"
		: : [pxTCB] "r" (pxCurrentTCB), [svcsp] "r" (puxSVCStackPointer) :
	);
}


void vPortSMCHandler( void )
{
	/* Nothing to do. */
}


void vPortYieldFromISR( void )
{
	vTaskSwitchContext();
	portSGI_CLEAR_YIELD( portGIC_DISTRIBUTOR_BASE, portCORE_ID() );
}


/*
 * See header file for description.
 */
portBASE_TYPE xPortStartScheduler( void )
{
	/* Start the timer that generates the tick ISR.  Interrupts are disabled
	here already. */
	prvSetupTimerInterrupt();

	/* Install the interrupt handler. */
	vPortInstallInterruptHandler( (void (*)(void *))vPortYieldFromISR, NULL, portSGI_YIELD_VECTOR_ID, pdTRUE, /* configMAX_SYSCALL_INTERRUPT_PRIORITY */ configKERNEL_INTERRUPT_PRIORITY, 1<<portCORE_ID() );

	/* Finally, allow the GIC to pass interrupts to the processor. */
	portGIC_WRITE( portGIC_ICDDCR(portGIC_DISTRIBUTOR_BASE), 0x01UL );
	portGIC_WRITE( portGIC_ICCBPR(portGIC_PRIVATE_BASE), 0x00UL );
	portGIC_WRITE( portGIC_ICCPMR(portGIC_PRIVATE_BASE), configLOWEST_INTERRUPT_PRIORITY );
	portGIC_WRITE( portGIC_ICCICR(portGIC_PRIVATE_BASE), 0x01UL );

	/* Start the first task. */
	vPortStartFirstTask();

	/* Should not get here! */
	return 0;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* It is unlikely that the CM3 port will require this function as there
	is nothing to return to.  */
}
/*-----------------------------------------------------------*/

void vPortSysTickHandler( void *pvParameter )
{
	/* Clear the Interrupt. */
	*(portSYSTICK_INTERRUPT_STATUS) = 0x01UL;

	vTaskIncrementTick();

#if configUSE_PREEMPTION == 1
	/* If using preemption, also force a context switch. */
	portEND_SWITCHING_ISR(pdTRUE);
#endif
}
/*-----------------------------------------------------------*/

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
void prvSetupTimerInterrupt( void )
{
	/* Install the interrupt handler. */
	vPortInstallInterruptHandler( vPortSysTickHandler, NULL, portSYSTICK_VECTOR_ID, pdTRUE, /* configMAX_SYSCALL_INTERRUPT_PRIORITY */ configKERNEL_INTERRUPT_PRIORITY, 1<<portCORE_ID() );

	/* Configure SysTick to interrupt at the requested rate. */
	*(portSYSTICK_LOAD) = (configCPU_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
	*(portSYSTICK_CONTROL) = ( portSYSTICK_PRESCALE << 8 ) | portSYSTICK_CTRL_ENABLE_PERIODIC_INTERRUPTS;

}


/* ISR */
void vPortGICInterruptHandler( void )
{
	unsigned long ulVector = 0UL;
	unsigned long ulGICBaseAddress = portGIC_PRIVATE_BASE;

	/* Query the private address first. */
	ulVector = portGIC_READ( portGIC_ICCIAR(portGIC_PRIVATE_BASE) );

	if ( ( ( ulVector & portGIC_VECTOR_MASK ) < ulMaxVectorId ) && ( NULL != pxInterruptHandlers[ ( ulVector & portGIC_VECTOR_MASK ) ].vHandler ) )
	{
		/* Call the associated handler. */
		pxInterruptHandlers[ ( ulVector & portGIC_VECTOR_MASK ) ].vHandler( pxInterruptHandlers[ ( ulVector & portGIC_VECTOR_MASK ) ].pvParameter );
		/* And acknowledge the interrupt. */
		portGIC_WRITE( portGIC_ICCEOIR(ulGICBaseAddress), ulVector );
	}
	else
	{
		/* This is a spurious interrupt, do nothing. */
	}
}
/*----------------------------------------------------------------------------*/

portBASE_TYPE xPortSetInterruptMask( void )
{
portBASE_TYPE xPriorityMask = portGIC_READ( portGIC_ICCPMR(portGIC_PRIVATE_BASE) );
	portGIC_WRITE( portGIC_ICCPMR(portGIC_PRIVATE_BASE), configMAX_SYSCALL_INTERRUPT_PRIORITY );
	return xPriorityMask;
}
/*----------------------------------------------------------------------------*/

void vPortClearInterruptMask( portBASE_TYPE xPriorityMask )
{
	portGIC_WRITE( portGIC_ICCPMR(portGIC_PRIVATE_BASE), xPriorityMask );
}
/*----------------------------------------------------------------------------*/

void vPortUnknownInterruptHandler( void *pvParameter )
{
	/* This is an unhandled interrupt, do nothing. */
	(void)pvParameter;
}
/*----------------------------------------------------------------------------*/


/* System Control Register, p4-71 */
static unsigned long ReadSCTLR()
{
	unsigned long SCTLR;
	__asm volatile("mrc p15, 0, %[sctlr], c1, c0, 0"
	:[sctlr] "=r" (SCTLR)::);
	return SCTLR;
}

static void WriteSCTLR(unsigned long SCTLR)
{
	__asm volatile("mcr p15, 0, %[sctlr], c1, c0, 0"
	::[sctlr] "r" (SCTLR):);
}

static unsigned long ReadACTLR()
{
	unsigned long ACTLR;
	__asm volatile("mrc p15, 0, %[actlr], c1, c0, 1"
	:[actlr] "=r" (ACTLR)::);
	return ACTLR;
}

static void WriteACTLR(unsigned long ACTLR)
{
	__asm volatile("mcr p15, 0, %[actlr], c1, c0, 0"
	::[actlr] "r" (ACTLR):);
}

void _init(void)
{
	int i;
	unsigned long *pulSrc, *pulDest;
	volatile unsigned long ulSCTLR = 0UL;
	
extern unsigned long __isr_vector_start;
extern unsigned long __isr_vector_end;
extern unsigned long _bss;
extern unsigned long _ebss;

	/* Zero fill the bss segment. */
	for(pulDest = &_bss; pulDest < &_ebss; )
	{
		*pulDest++ = 0;
	}

	/* Configure the Stack Pointer for the Processor Modes. */
	__asm volatile (
			" cps #17							\n"
			" nop 								\n"
			" mov SP, %[fiqsp]					\n"
			" nop 								\n"
			" cps #18							\n"
			" nop 								\n"
			" mov SP, %[irqsp]					\n"
			" nop 								\n"
			" cps #23							\n"
			" nop 								\n"
			" mov SP, %[abtsp]					\n"
			" nop 								\n"
			" cps #27							\n"
			" nop 								\n"
			" mov SP, %[abtsp]					\n"
			" nop 								\n"
			" cps #19							\n"
			" nop 								\n"
			" mov SP, %[svcsp]					\n"
			" nop 								\n"
			: : [fiqsp] "r" (puxFIQStackPointer),
				[irqsp] "r" (puxIRQStackPointer),
				[abtsp] "r" (puxAbortStackPointer),
				[svcsp] "r" (puxSVCStackPointer)
				:  );

	/* Finally, copy the exception vector table over the boot loader. */
	pulSrc = (unsigned long *)&__isr_vector_start;
	pulDest = (unsigned long *)portEXCEPTION_VECTORS_BASE;
	for ( pulSrc = &__isr_vector_start; pulSrc < &__isr_vector_end; )
	{/* MPCore */
		*pulDest++ = *pulSrc++;
	}

	/* VBAR is modified to point to the new Vector Table. */
	pulDest = (unsigned long *)portEXCEPTION_VECTORS_BASE;
	__asm volatile(
			" mcr p15, 0, %[vbar], c12, c0, 0 			\n"
			: : [vbar] "r" (pulDest) :
			);

	/* Now we modify the SCTLR to change the Vector Table Address. */
	ulSCTLR=ReadSCTLR();
	ulSCTLR&=~(1<<13);	/* clear V bit : base address of vector is 0x0000 0000 */
	WriteSCTLR(ulSCTLR);

	// Enable branch prediction.
	WriteSCTLR(ReadSCTLR()|(1<<11));
  
	// Enable D-side prefetch. p4-81
	WriteACTLR(ReadACTLR()|(1<<2));

	// Set up page table.
	for(i=0;i<64;i++)
	{
		PageTable[i]=(i<<20)|0x05de6;
	}

	for(i=64;i<4096;i++)
	{
		PageTable[i]=(i<<20)|0x0de2;
	}

	// Initialize MMU.

	// Enable TTB0 only.
	// Set translation table base address.
	// Set all domains to client.
	__asm volatile (
			" mcr	p15, 0, %[ttbcr], c2, c0, 2		\n" // Write Translation Table Base Control Register  LDR   r0, ttb_address
			" mcr	p15, 0, %[ttbr0], c2, c0, 0	\n" // Write Translation Table Base Register 0
			" mcr	p15, 0, %[dacr], c3, c0, 0 	\n" // Write Domain Access Control Register
			: : [ttbcr] "r" (0),
				[ttbr0] "r" (PageTable),
				[dacr] "r" (0x55555555)
				:  );

	// Enable MMU.
	WriteSCTLR(ReadSCTLR()|(1<<0));

	// Enable L1 I & D caches.
	WriteSCTLR(ReadSCTLR()|(1<<2)|(1<<12));

	main();
}

void Undefined_Handler_Panic( void )
{/* secure monitor call (SMC) instruction */
	xUARTSendCharacter(portCORE_ID(), 'U',0); /* after print out OK, this can be used in ISR */
	__asm volatile ( " smc #0 " );
	for (;;);
}

void Prefetch_Handler_Panic( void )
{
	__asm volatile ( " smc #0 " );
	for (;;);
}

void Abort_Handler_Panic( void )
{
	xUARTSendCharacter(portCORE_ID(), 'A',0);

	for (;;);
}

