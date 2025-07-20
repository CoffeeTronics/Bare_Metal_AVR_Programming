#include "usart0_tx.h"

#define BAUD_RATE 1200

FILE USART_stream = FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);


void USART_Init(USART_CMODE_t cmode, USART_PMODE_t pmode, USART_SBMODE_t sbmode , USART_CHSIZE_t chsize)   {
    USART0.CTRLC |= cmode | pmode | sbmode | chsize;
    USART0.BAUD = USART0_BAUD_RATE(BAUD_RATE);
    PORTD.DIRSET |= PIN4_bm;
    PORTMUX.USARTROUTEA = PORTMUX_USART0_ALT3_gc;
    USART0.CTRLB |= USART_TXEN_bm;
    stdout = &USART_stream;
    for(int i=0; i<35; i++);
}

void USART0_sendChar(char c)    {
    while (!(USART0.STATUS & USART_DREIF_bm));       
    USART0.TXDATAL = c;
}


int USART0_printChar(char character, FILE *stream)  {
    USART0_sendChar(character);
    return 0;
}