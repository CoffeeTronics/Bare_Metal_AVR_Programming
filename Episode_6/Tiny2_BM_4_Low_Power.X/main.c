
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












///*
// * File:   main.c
// * Author: C52467
// *
// * Created on December 15, 2021, 11:11 AM
// */
//
///* This example shows how to use TCA and interrupts to 
// * toggle the onboard LED */
//
//#include <xc.h>
//#include <avr/interrupt.h>
//
//#include <avr/xmega.h>
//
//// function prototypes
//void init_TCA(void);
//void init_port(void);
//
//// global variable
//volatile uint16_t counter = 0;
//
//int main(void) {
//
//    init_TCA();
//    init_port();
//    
//    sei();  // enable global interrupts
//    
//    while(1)    {
//        // Reset counter and toggle LED every 1000ms
//        if (counter >= 1000) {
//            PORTB.OUTTGL = PIN7_bm;// | PIN3_bm; 
//            counter = 0;
//        }   
//    }
//    return 0;
//}
//
//ISR(TCA0_OVF_vect)  {
//    counter++;
//    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;   // reset interrupt
//}
//
//void init_port(void)    {
//    PORTB.DIR = PIN7_bm;// | PIN3_bm;  // set PORTB7 (LED0) as output
//    
//    // disable all other pins to reduce power consumption
//    PORTA.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTA.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTA.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTA.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTA.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTA.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTA.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTA.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    
//    PORTB.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTB.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTB.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTB.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTB.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTB.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTB.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    
//    PORTC.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTC.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTC.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTC.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTC.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTC.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
//    PORTC.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
//}
//void init_TCA(void) {
//    
//    // no waveform generated
//    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;     
//    
//    // enable ovf 
//    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;   
//    
//    // set PER register
//    // CLK_per = 1 / 3.333333MHz = 300ns
//    // PER = 1ms / 300ns = 3333
//    TCA0.SINGLE.PER = 3255; 
//    // start timer
//    TCA0.SINGLE.CTRLA |= TCA_SINGLE_CLKSEL_DIV1024_gc | 
//                        TCA_SINGLE_ENABLE_bm |  
//                        TCA_SINGLE_RUNSTDBY_bm; // enable TCA0
//}
