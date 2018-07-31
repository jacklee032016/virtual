
#ifndef	__CORTEX_A9_H__
#define	__CORTEX_A9_H__


extern void vPortUnknownInterruptHandler( void *pvParameter );
/* Interrupt Handler code. */
void vPortInstallInterruptHandler( void (*vHandler)(void *), void *pvParameter, unsigned long ulVector, unsigned char ucEdgeTriggered, unsigned char ucPriority, unsigned char ucProcessorTargets );

/* from UART or pl1011 */
extern void vUARTInitialise(unsigned long ulUARTPeripheral, unsigned long ulBaud, unsigned long ulQueueSize );

extern portBASE_TYPE xUARTReceiveCharacter( unsigned long ulUARTPeripheral, signed char *pcChar, portTickType xDelay );
extern portBASE_TYPE xUARTSendCharacter( unsigned long ulUARTPeripheral, signed char cChar, portTickType xDelay );

extern int main( void );


#endif

