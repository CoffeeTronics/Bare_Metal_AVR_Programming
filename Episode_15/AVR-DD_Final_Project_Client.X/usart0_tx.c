#include "usart0_tx.h"

#define BAUD_RATE 1200UL    

FILE USART_stream = FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);


void USART_Init(USART_CMODE_t cmode, USART_PMODE_t pmode, USART_SBMODE_t sbmode , USART_CHSIZE_t chsize)   {
    // Set Comms Mode, Parity, Number of Stop Bits, and Number of Data Bits
    USART0.CTRLC |= cmode | pmode | sbmode | chsize;
    // Set required BAUD rate - User definable on line 3 of this file
    USART0.BAUD = USART0_BAUD_RATE(BAUD_RATE);
    // Use Alternative Tx pin PD4  - See datasheet pg 157
    PORTMUX.USARTROUTEA = PORTMUX_USART0_ALT3_gc;
    // Enable USART0 Tx
    USART0.CTRLB |= USART_TXEN_bm;
    // Redirect stdout stream to use printf
    stdout = &USART_stream;
    // delay to stabilize USART before using printf
    for(int i=0; i<500; i++);   
}

// Send an individual character. This is used by USART0_printChar as part of printf functionality
void USART0_sendChar(char c) 
{
    while (!(USART0.STATUS & USART_DREIF_bm));       
    USART0.TXDATAL = c;
}

// USART0_printChar is part of printf functionality
int USART0_printChar(char character, FILE *stream)
{
    USART0_sendChar(character);
    return 0;
}