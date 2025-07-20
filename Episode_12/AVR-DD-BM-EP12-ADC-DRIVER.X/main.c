/*
 * File:   main.c
 * Author: Ross Satchell
 *
 * Created on August 9, 2023, 2:27 PM
 */
#define F_CPU 1E6   

#include <xc.h>
#include "usart0_tx.h"
#include "main_clk_ctrl.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "adc.h"


//// function prototypes
void Init_Ports(void);


int main(void) {
    
    //Clock Selection, Freq select, Prescaler Enable, Prescaler Divider, Clock out
    MainClkCtrl(CLKCTRL_CLKSEL_OSCHF_gc, CLKCTRL_FRQSEL_1M_gc, 0, 0, 1);
    USART_Init(USART_CMODE_ASYNCHRONOUS_gc, USART_PMODE_DISABLED_gc, USART_SBMODE_1BIT_gc, USART_CHSIZE_8BIT_gc);

    printf("\nUSART Driver Initialization Success!\n");
    
    // parameters: vref, vref_alwayson, runstdby, convmode, leftadj, ressel, 
    // freerun, initdly, sampnum, presc, sampdly, 
    // sample length, muxpos, muxneg, window cmp, window low th, window hi th    
    ADC_Init(VREF_REFSEL_VDD_gc, 1, 1, ADC_CONVMODE_SINGLEENDED_gc, 0, ADC_RESSEL_12BIT_gc,
            0, ADC_INITDLY_DLY16_gc, ADC_SAMPNUM_NONE_gc, ADC_PRESC_DIV2_gc, ADC_SAMPDLY_DLY0_gc, 
            2, ADC_MUXPOS_AIN18_gc, ADC_MUXNEG_GND_gc, ADC_WINCM_INSIDE_gc, 1000, 3000);
    ADC_Enable();
    printf("ADC Initialised and enabled!\n");
    
    // Single Sampled ADC with Window
    while(1)   {
        ADC_Start_Conversion();
        // Wait for conversion to be complete 
        while (!(ADC_Is_Conversion_Done()));
        
        // Checks for Window Comparison
        if (ADC_Is_Window_Satisfied()) {
            printf("Window Conditions Satisfied: %d\n", ADC_Get_Result());
        }
        else printf("Window Conditions NOT Satisfied: %d\n", ADC_Get_Result());
        
        _delay_ms(100);
    }
    return 0;
    

    // Accumulated Sampled ADC With Window
    while(1)    {
        ADC_Start_Oversampled_Conversion(ADC_SAMPNUM_ACC16_gc);
        // Wait for conversion to be complete 
        while (!(ADC_Is_Conversion_Done())); 
        // Checks for Window Comparison
        if (ADC_Is_Window_Satisfied()) {
            printf("Window Conditions Satisfied: %d\n", ADC_Get_Oversampled_Result(14));      
        }
        else printf("Window Conditions NOT Satisfied: %d\n", ADC_Get_Oversampled_Result(14)); 

        _delay_ms(100);
    }
    
    return 0;

}


void Init_Ports(void)   {
    // LED as output
    PORTF.DIRSET = PIN5_bm;
    PORTF.OUTCLR = PIN5_bm; // LED on (Active LOW)
}






