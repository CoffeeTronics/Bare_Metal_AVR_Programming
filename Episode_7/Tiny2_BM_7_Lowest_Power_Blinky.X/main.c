/*
 *  Tiny2 Bare Metal 7 - Interrupt Driven Blinking LED - lowest power possible
 */

#include <xc.h>
#include <avr/xmega.h>
#include <avr/interrupt.h>
//#include <avr/iotn1627.h>


// function prototypes
void MainClkCtrl(void);
void Init_Sleep(void);
void Init_Ports(void);
void Init_RTC(void);
void Init_PIT(void);

int main(void) {

    MainClkCtrl();
    Init_Ports();
    Init_RTC();		// uncomment this to test RTC
    //Init_PIT();   // uncomment this to test PIT
    Init_Sleep();
    
    sei();  

    while(1)    {
        asm("SLEEP"); // for active mode power measurements, comment this line out
    }
    
    return 0;
    
}

ISR(RTC_PIT_vect)   {
    //PORTB.OUTTGL = PIN7_bm;   // comment this out for lowest power sleep measurements
    RTC.PITINTFLAGS |= RTC_PI_bm;
}


ISR(RTC_CNT_vect)   {
    PORTB.OUTTGL = PIN7_bm;   // comment this out for lowest power sleep measurements
    RTC.INTFLAGS |= RTC_OVF_bm;
}

void MainClkCtrl(void)  {
    // USE INTERNAL 32kHz OSC
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_OSCULP32K_gc);
    // Turn off prescaler
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL.MCLKCTRLB & ~CLKCTRL_PEN_bm);
}

void Init_PIT(void) {
    RTC.CLKSEL = RTC_CLKSEL_INT1K_gc;   //RTC_CLKSEL_INT32K_gc; // tested with both 32kHz and 1kHz clock
    RTC.PITINTCTRL = RTC_PI_bm;
    
    while(RTC.PITSTATUS & RTC_CTRLBUSY_bm); //wait until not busy to write
    // tested with PIT period of both 32.768kHz and 1.024kHz clocks
    RTC.PITCTRLA = RTC_PERIOD_CYC1024_gc | RTC_PITEN_bm; //RTC_PERIOD_CYC32768_gc | RTC_PITEN_bm;
    
}

void Init_RTC(void) {
    
    // tested with RTC clock of both 32.768kHz and 1.024kHz 
    RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;// RTC_CLKSEL_INT1K_gc;   //RTC_CLKSEL_INT32K_gc;
    while(RTC.STATUS & RTC_PERBUSY_bm); // wait until not busy to write
    RTC.PER = 32768; //1024;    // tested with period of both 1024 & 32768
        
    RTC.INTCTRL = RTC_OVF_bm;
    
    while (RTC.CTRLA & RTC_CTRLABUSY_bm);   // wait until CTRLA not busy in STATUS register
    RTC.CTRLA = RTC_RUNSTDBY_bm | RTC_PRESCALER_DIV1_gc | RTC_RTCEN_bm ;  // prescaler 1, run in standby mode and start RTC
}

void Init_Ports(void)   {
    
    //PORTB.DIR = PIN7_bm;  // used when toggling LED, commented out for low power measurements
    PORTB.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    
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
}

void Init_Sleep(void)   {
    // uncomment desired sleep mode to test
    
    //SLPCTRL.CTRLA = SLPCTRL_SMODE_IDLE_gc | SLPCTRL_SEN_bm;
    //SLPCTRL.CTRLA = SLPCTRL_SMODE_STDBY_gc | SLPCTRL_SEN_bm;    
    SLPCTRL.CTRLA = SLPCTRL_SMODE_PDOWN_gc | SLPCTRL_SEN_bm;
}

