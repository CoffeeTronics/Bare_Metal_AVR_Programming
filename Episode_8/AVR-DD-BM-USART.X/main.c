/*
 * File:   main.c
 * Author: Ross Satchell
 *
 * Created on January 18, 2023, 2:27 PM
 */


#include <xc.h>
#include <stdio.h>
//#include <avr/ioavr64dd32.h>

// function prototypes
void MainClkCtrl(void);
void USART_Init(void);
void USART0_sendChar(char);
int USART0_printChar(char, FILE*);

FILE USART_stream = FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);

#define CLK_PER 32768
#define USART0_BAUD_RATE(BAUD_RATE) (((float) CLK_PER * 64.0 / (16.0 * (float) BAUD_RATE))+ 0.5)



int main(void) {
    PORTA.DIRSET = PIN7_bm;
    
    MainClkCtrl();
    USART_Init();
    uint8_t printVal = 0x00;
    
    while(1)    {
        printf("Hello World: %d\n", printVal++);
    }
    
    return 0;
}


void MainClkCtrl(void)  {
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_OSC32K_gc | CLKCTRL_CLKOUT_bm);
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL.MCLKCTRLB & ~ CLKCTRL_PEN_bm);
}


void USART_Init(void)   {
    USART0.CTRLC |= USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc | USART_SBMODE_1BIT_gc;
    USART0.BAUD = USART0_BAUD_RATE(1200);
    PORTD.DIRSET |= PIN4_bm;
    PORTMUX.USARTROUTEA = PORTMUX_USART0_ALT3_gc;
    USART0.CTRLB |= USART_TXEN_bm;
    stdout = &USART_stream;
}

void USART0_sendChar(char c) 
{
    while (!(USART0.STATUS & USART_DREIF_bm));       
    USART0.TXDATAL = c;
}


int USART0_printChar(char character, FILE *stream)
{
    USART0_sendChar(character);
    return 0;
}