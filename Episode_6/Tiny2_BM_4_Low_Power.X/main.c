
/*
 *  Tiny2 Bare Metal EP.5 - Interrupt Driven Blinking LED
 */

#include <xc.h>
#include <avr/xmega.h>
#include <avr/interrupt.h>
//#include <avr/iotn1627.h> // uncomment to access header file, dont forget to comment out before building



// function prototypes
void MainClockCtrl(void);
void InitPorts(void);
void InitTCA(void);
void TCA_InterruptWait(void);
void InitSleep(void);

int main(void) {
    
    MainClockCtrl();
    InitPorts();
    InitTCA();
    InitSleep();
    
    sei();

    while(1)    {
        asm ("SLEEP");
    }
    
    return 0;
    
}

ISR (TCA0_OVF_vect)  {
    // toggle LED on PB7
    PORTB.OUTTGL = PIN7_bm;
    // reset OVF flag to catch next interrupt
    TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_OVF_bm;

    
}

void MainClockCtrl(void)    {
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_OSC20M_gc | CLKCTRL_CLKOUT_bm);
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_6X_gc | CLKCTRL_PEN_bm);
    // 20MHz Clock and Prescaler of 6 => CLK_PER = 3.333333MHz = 300ns PER_CLK 
}

void InitPorts(void)   {
    // set PB7 as output
    PORTB.DIR = PIN7_bm;
    
    PORTA.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTA.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTA.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTA.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    
    PORTB.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTB.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTB.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTB.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTB.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTB.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTB.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    
    PORTC.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTC.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTC.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTC.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTC.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTC.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTC.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
}

void InitTCA(void) {
    // Waveform Generation Normal mode
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.PER = 3255; 
    
    // CLK_PER = 300ns. TCA clock divider of 1024, coupled with TCA PER of 3256 should give OVF time of 1 sec.
    // Enable TCA in Run Standby mode to enable during sleep, start TCA
    TCA0.SINGLE.CTRLA =  TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm | TCA_SINGLE_RUNSTDBY_bm;
}


void InitSleep(void)    {
    // Sleep - Standby mode selected, Sleep enabled
    SLPCTRL.CTRLA = SLPCTRL_SMODE_STDBY_gc | SLPCTRL_SEN_bm;
}
