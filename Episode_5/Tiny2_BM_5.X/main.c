/*
 *  Tiny2 Bare Metal EP.5 - Interrupt Driven Blinking LED
 */

#include <xc.h>
#include <avr/xmega.h>
#include <avr/interrupt.h>
//#include <avr/iotn1627.h>


// function prototypes
void MainClkCtrl(void);
void Init_Sleep(void);
void Init_Ports(void);
void Init_TCA(void);

int main(void) {

    MainClkCtrl();
    Init_Ports();
    Init_TCA();
    Init_Sleep();
    
    sei();  

    while(1)    {
        asm("SLEEP");
    }
    
    return 0;
    
}

ISR(TCA0_OVF_vect)  {
    PORTB.OUTTGL = PIN7_bm;
    TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_OVF_bm;
}

void MainClkCtrl(void)  {
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_OSC20M_gc);// | CLKCTRL_CLKOUT_bm);
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_6X_gc | CLKCTRL_PEN_bm);
}

void Init_TCA(void) {
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.PER = 3255;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | 
            TCA_SINGLE_ENABLE_bm |
            TCA_SINGLE_RUNSTDBY_bm;
}

void Init_Ports(void)   {
    PORTB.DIR = PIN7_bm;       
}

void Init_Sleep(void)   {
    SLPCTRL.CTRLA = SLPCTRL_SMODE_STDBY_gc | SLPCTRL_SEN_bm;
    //SLPCTRL.CTRLA = SLPCTRL_SMODE_IDLE_gc | SLPCTRL_SEN_bm;
    
}

