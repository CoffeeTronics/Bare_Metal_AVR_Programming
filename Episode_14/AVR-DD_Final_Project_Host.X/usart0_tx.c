#include "usart0_tx.h"

#define BAUD_RATE 1200UL    // User to define requried BAUD rate here

// File stream to enable printf functionality
FILE USART_stream = FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);

// Initialize USART peripheral
void USART_Init(USART_CMODE_t cmode, USART_PMODE_t pmode, USART_SBMODE_t sbmode , USART_CHSIZE_t chsize)   {
    USART0.CTRLC |= cmode | pmode | sbmode | chsize;
    USART0.BAUD = USART0_BAUD_RATE(BAUD_RATE);
    PORTD.DIRSET |= PIN4_bm;
    PORTMUX.USARTROUTEA = PORTMUX_USART0_ALT3_gc;
    USART0.CTRLB |= USART_TXEN_bm;
    stdout = &USART_stream;
    // delay to stabilize USART before using printf
    for(int i=0; i<500; i++);   
}

// Send an individual character. This is used by USART0_printChar as part of printf functionality
void USART0_sendChar(char c)    {
    while (!(USART0.STATUS & USART_DREIF_bm));       
    USART0.TXDATAL = c;
}

// USART0_printChar is part of printf functionality
int USART0_printChar(char character, FILE *stream)  {
    USART0_sendChar(character);
    return 0;
}