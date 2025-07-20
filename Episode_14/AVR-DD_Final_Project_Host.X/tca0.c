#include "tca0.h"
#include <avr/interrupt.h>


void TCA0_Init(uint16_t period)    {
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
    TCA0.SINGLE.PER = period;   // set period
    TCA0.SINGLE.INTCTRL |= TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_RUNSTDBY_bm | 
            TCA_SINGLE_CLKSEL_DIV8_gc | 
            TCA_SINGLE_ENABLE_bm;   // enable TCA to start timer
}

void TCA0_Enable(void)   {
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;  // enable TCA to start timer
}

void TCA0_Disable(void)  {
    TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm; // disable TCA to stop timer
}

ISR(TCA0_OVF_vect)   {
    PORTF.OUTTGL |= PIN5_bm;
    TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_OVF_bm;
}
