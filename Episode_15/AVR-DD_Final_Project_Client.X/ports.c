#include "ports.h"
#include "spi0.h"
#include <avr/interrupt.h>

/* IMPORTANT NOTE: 
 Pins in use are as follows:
 *  PORTA - pins 4,5,6,7 for SPI MOSI, MISO, SCK, _SS respectively
 *  PORTC - None
 *  PORTD - None
 *  PORTF - PF5 as USART output
 */

// Software flag that is set when Pinc Change ISR on SPI _SS (PA7) goes LOW
volatile uint8_t client_select_pulled_low_flag = 0; 

// Initialize all PORT pins for this application
void PORT_Init(void)   {

    //  Set PORTC, PORTD and PORTF as ALL OUTPUTS
    PORTC.DIRSET = 0xFF;
    PORTD.DIRSET = 0xFF;
    PORTF.DIRSET = 0x20; // set PF5 as output    
    
    // SPI pin config: MOSI(PA4), MISO(PA5), SCK(PA6), _SS(PA7)
    PORTA.DIRSET = PIN5_bm;                     // Set SPI pin MISO(PA5) as an output
    PORTA.DIRCLR = PIN4_bm | PIN6_bm | PIN7_bm; // Set SPI pins MOSI(PA4), SCK(PA6), _SS(PA7) as inputs
    
    // Multi-Pin configuration - See datasheet pg 181 - These are the settings
    PORTA.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTC.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTD.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTF.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    
    // Multi-Pin configuration - See datasheet pg 182 - These are the pins to apply the above PINCONFIG settings to
    PORTA.PINCTRLUPD = 0x0F;  // Copy above PINCONFIG to all PORTA pins EXCEPT pins PA4,PA5,PA6,PA7
    PORTC.PINCTRLUPD = 0xFF;  // Copy above PINCONFIG to all PORTC pins
    PORTD.PINCTRLUPD = 0xEF;  // Copy above PINCONFIG to all PORTD pins EXCEPT pin PD4
    PORTF.PINCTRLUPD = 0xFF;  // Copy above PINCONFIG to all PORTF pins             
    
    PORTA.OUTSET = 0x0F;    // Tie all unused pins HIGH (PA4,PA5,PA6,PA7 are used for SPI))
    PORTC.OUTSET = 0xFF;    // Tie all PORTF pins HIGH
    PORTD.OUTSET = 0xEF;    // tie all unused pins HIGH (PD4 is used for USART)
    PORTF.OUTSET = 0xFF;    // Tie all PORTF pins HIGH
        
    PORTA.PIN7CTRL |= PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm;    // Enable _SS interrupt on falling edge, and enable internal pullup
    
}

// Getter function for client_select_pulled_low_flag software flag
uint8_t GetClientSelectFlagStatus(void) {
    return client_select_pulled_low_flag;
}

// Clear function for client_select_pulled_low_flag software flag
void ClrClientSelectFlag(void) {
    client_select_pulled_low_flag = 0;
}

// SPI _SS (PA7) line pulled LOW interrupt
ISR(PORTA_PORT_vect)   {
    if(PORTA.INTFLAGS & PIN7_bm)    {
        PORTA.INTFLAGS = PIN7_bm;           // clear pin change interrupt
        client_select_pulled_low_flag = 1;  // Set software flag
    }
}
