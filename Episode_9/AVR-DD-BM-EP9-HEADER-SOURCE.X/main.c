/*
 * File:   main.c
 * Author: Ross Satchell
 *
 * Created on January 18, 2023, 2:27 PM
 */


#include <xc.h>
#include "usart0_tx.h"
#include "main_clk_ctrl.h"


int main(void) {
    
    // 32.768kHz Internal Oscillator, No Freq Select, No Prescaler Enable, No Prescaler Divider, Clock Out Enabled
    MainClkCtrl(CLKCTRL_CLKSEL_OSC32K_gc, 0, 0, 0, 1);
    
    // Async, No Parity, 1 Stop Bit, 8 Data Bits
    USART_Init(USART_CMODE_ASYNCHRONOUS_gc, USART_PMODE_DISABLED_gc, USART_SBMODE_1BIT_gc, USART_CHSIZE_8BIT_gc);

    printf("Driver Initialization Success!\n");
    
    while(1)    {
        
    }
    
    return 0;
}




