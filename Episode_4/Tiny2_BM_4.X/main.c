/*
 * File:   main.c
 * Author: C52467
 *
 * Created on July 18, 2025, 7:21 PM
 */


#include <xc.h>
//#include <avr/iotn1627.h> // uncomment to access header file, dont forget to comment out before building

void main(void) {
    uint16_t counter = 0;
    PORTB.DIR = PIN7_bm;
    
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
    TCA0.SINGLE.PER = 3333;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
            
    while(1)    {
        if (TCA0.SINGLE.INTFLAGS & TCA_SINGLE_OVF_bm)   {
            TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_OVF_bm;
            counter++;
        }
        if (counter >- 1000)    {
            PORTB.OUTTGL = PIN7_bm;
            counter = 0;
        }
    }
    return;
}
