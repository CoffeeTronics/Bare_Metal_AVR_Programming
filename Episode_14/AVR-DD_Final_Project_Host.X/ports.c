#include "ports.h"
#include <avr/interrupt.h>

volatile uint8_t button_pressed = 0;


void PORT_Init(void)   {

    //  Set PORTA, PORTC and PORTD as ALL OUTPUTS
    PORTA.DIRSET = 0xFF;
    PORTC.DIRSET = 0xFF;
    PORTD.DIRSET = 0xFF;
    
    // Set all PORTF pins (except PIN6) as outputs
    PORTF.DIRSET = PIN7_bm | PIN5_bm | PIN4_bm | PIN3_bm | PIN2_bm | PIN1_bm | PIN0_bm;
    PORTF.DIRCLR |= PIN6_bm; // Set user button  switch on PF6 as input
    
    // Multi-Pin configuration - See datasheet pg 181
    PORTA.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTC.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTD.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTF.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    
    // Multi-Pin configuration - See datasheet pg 182
    PORTA.PINCTRLUPD = 0xFF;
    PORTC.PINCTRLUPD = 0xFF;
    PORTD.PINCTRLUPD = 0xFF;
    PORTF.PINCTRLUPD = 0xBF;    // Copy above PINCONFIG to all PORTF pins EXCEPT pin 6
    
    PORTA.OUTSET = 0xFF;    // tie all unused pins HIGH
    PORTC.OUTSET = 0xFF;    // tie all unused pins HIGH
    PORTD.OUTSET = 0xFF;    // tie all unused pins HIGH
    PORTF.OUTSET = 0xBF;    // tie all unused pins HIGH except pin PF6 (button switch)
            
    PORTF.PIN6CTRL = PORT_ISC_RISING_gc | PORT_PULLUPEN_bm;    // user button interrupt on rising edge for pin PF6
}

// Reduces power consumption when called before SLEEP
void TurnOffUnusedPinsBeforeSleep(void)    { 
    // PORTA pins and LED pin (PF5) are the only ones that need to be put into low power mode, 
    // as everything else is either already in low-power mode, or needs to detect a pin change interrupt (button switch on PF6)
    PORTA.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTA.PINCTRLUPD = 0xFF;
}

// Pin change interrupt for button switch (PF6))
ISR(PORTF_PORT_vect)   {
    if(PORTF.INTFLAGS & PIN6_bm)    {
        PORTF.INTFLAGS = PIN6_bm;   // clear pin change interrupt
        button_pressed = 1;
    }
}

// Get status of volatile button_pressed variable
uint8_t GetButtonPressedStatus(void)    {
    return button_pressed;
}

// Clear status of volatile button_pressed variable
void ClrButtonPressedStatus(void)   {
    button_pressed = 0;
}
    