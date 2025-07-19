/*
 * File:   main.c
 * Author: Ross Satchell
 *
 * Created on January 31, 2022, 3:09 PM
 */

#define F_CPU 3333333UL
#include <xc.h>
//#include <avr/iotn1627.h> // uncomment to access header file, dont forget to comment out before building
#include <util/delay.h>


void main(void) {
    
    PORTB_DIR = PIN7_bm;

    
    while(1)    {
        PORTB_OUTTGL = PIN7_bm;
		
        _delay_ms(1000);
    }

    
    return;
}
