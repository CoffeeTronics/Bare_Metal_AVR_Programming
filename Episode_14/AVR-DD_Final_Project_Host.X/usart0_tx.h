// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USART0_TX_H
#define	USART0_TX_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>

// function prototypes
void USART_Init(USART_CMODE_t, USART_PMODE_t, USART_SBMODE_t, USART_CHSIZE_t);
void USART0_sendChar(char);
int USART0_printChar(char, FILE*);

// IMPORTANT NOTE: define main clock freq here - this is for USART BAUD calculation
#define CLK_PER 32768UL 
#define USART0_BAUD_RATE(BAUD_RATE) (((float) CLK_PER * 64.0 / (16.0 * (float) BAUD_RATE))+ 0.5)


#endif	/* USART0_TX_H */

