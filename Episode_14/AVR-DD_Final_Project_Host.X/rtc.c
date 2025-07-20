#include "rtc.h"
#include <avr/interrupt.h>


void PIT_Init(void) {
    RTC.CLKSEL = RTC_CLKSEL_OSC32K_gc;
    RTC.PITINTCTRL = RTC_PI_bm;
    while(RTC.PITSTATUS & RTC_CTRLBUSY_bm); //wait until not busy to write
    RTC.PITCTRLA = RTC_PERIOD_CYC32768_gc | RTC_PITEN_bm;
}

void RTC_Init(void) {
    
    RTC.CLKSEL = RTC_CLKSEL_OSC32K_gc;
    while(RTC.STATUS & RTC_CMPBUSY_bm); // wait until not busy to write
    RTC.PER = 1024;
        
    RTC.INTCTRL = RTC_OVF_bm;
    
    while (RTC.CTRLA & RTC_CTRLABUSY_bm);   // wait until CTRLA not busy in STATUS register
    RTC.CTRLA = RTC_PRESCALER_DIV32_gc | RTC_RTCEN_bm | RTC_RUNSTDBY_bm;  // prescaler 32768, run in standby mode and start RTC
}

ISR(RTC_CNT_vect)   {
    PORTF.OUTTGL = PIN5_bm;
    RTC.INTFLAGS |= RTC_OVF_bm;
}


