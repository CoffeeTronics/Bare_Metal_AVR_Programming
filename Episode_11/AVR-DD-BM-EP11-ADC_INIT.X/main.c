/*
 * File:   main.c
 * Author: Ross Satchell
 *
 * Created on May 20, 2023, 2:27 PM
 */
#define F_CPU 1E6   // 32768UL //

#include <xc.h>
#include "usart0_tx.h"
#include "main_clk_ctrl.h"
#include <util/delay.h>
#include <avr/interrupt.h>
//#include "adc.h"


//// function prototypes
//
//// ADC_Init parameters: vref, vref_alwayson, runstdby, convmode, leftadj, ressel, freerun, initdly, 
//// sampnum, presc, sampdly, muxpos, muxneg, window cmp, window upper accumulated threshold, window lower accumulated threshold    
void ADC_Init(VREF_REFSEL_t, uint8_t, uint8_t, ADC_CONVMODE_t, uint8_t, ADC_RESSEL_t, uint8_t, ADC_INITDLY_t, 
        ADC_SAMPNUM_t, ADC_PRESC_t, ADC_SAMPDLY_t, uint8_t, ADC_MUXPOS_t, ADC_MUXNEG_t, ADC_WINCM_t, uint16_t winlt, uint16_t winht);
void ADC_Enable(void);
void ADC_Disable(void);
uint16_t ADC_Get_Result(void);
uint16_t ADC_Get_Oversampled_Result(uint8_t);
uint8_t ADC_Is_Conversion_Done(void);
void ADC_Start_Conversion(void);
void ADC_Stop_Conversion(void);
void ADC_Start_Oversampled_Conversion(ADC_SAMPNUM_t);
uint8_t ADC_Is_Window_Satisfied(void);
void Init_Ports(void);

uint16_t adc_read_val = 0;

uint8_t bitShift = 0;
uint8_t enob = 0;

int main(void) {
    
    //Clock Selection, Freq select, Prescaler Enable, Prescaler Divider, Clock out
    //MainClkCtrl(CLKCTRL_CLKSEL_OSC32K_gc, 0, 0, 0, 1);
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

  /*
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
*/  
}


void Init_Ports(void)   {
    // LED as output
    PORTF.DIRSET = PIN5_bm;
    PORTF.OUTCLR = PIN5_bm; // LED on (Active LOW)
}

void ADC_Init(VREF_REFSEL_t vref, uint8_t vref_alwayson, uint8_t runstdby, ADC_CONVMODE_t convmode, uint8_t leftadj, 
        ADC_RESSEL_t ressel, uint8_t freerun, ADC_INITDLY_t initdly, ADC_SAMPNUM_t sampnum, ADC_PRESC_t presc, 
        ADC_SAMPDLY_t sampdly, uint8_t samplen, ADC_MUXPOS_t muxpos, ADC_MUXNEG_t muxneg, ADC_WINCM_t wincm, uint16_t winlt, uint16_t winht) {
    
    VREF.ADC0REF = (vref << VREF_REFSEL_gp) | (vref_alwayson << VREF_ALWAYSON_bp);
    
    ADC0.CTRLA = (runstdby << ADC_RUNSTBY_bp) | (convmode << ADC_CONVMODE_bp) | (leftadj << ADC_LEFTADJ_bp) 
            | (ressel << ADC_RESSEL_gp) | (freerun << ADC_FREERUN_bp);
    
    ADC0.CTRLB = (sampnum << ADC_SAMPNUM_gp);   
    ADC0.CTRLC = (presc << ADC_PRESC_gp);   
    ADC0.CTRLD = ((sampdly << ADC_SAMPDLY_gp) | (initdly << ADC_INITDLY_gp));   
    ADC0.SAMPCTRL = samplen;
    if (convmode == ADC_CONVMODE_DIFF_gc)   {
        ADC0.MUXNEG = (muxneg << ADC_MUXNEG_gp);
    }
    ADC0.MUXPOS = (muxpos << ADC_MUXPOS_gp);  
    
    // if Window Cmp mode being used
    if (wincm != ADC_WINCM_NONE_gc) {
        // set Window Compare high and low thresholds
        ADC0.WINLT = winlt;
        ADC0.WINHT = winht;
        // set Window Compare mode 
        ADC0.CTRLE = (wincm << ADC_WINCM_gp); 
    }
}

void ADC_Enable(void) {
    ADC0.CTRLA |= (1 << ADC_ENABLE_bp);    
}

void ADC_Disable(void) {
    ADC0.CTRLA &= ~(1 << ADC_ENABLE_bp);
}

void ADC_Start_Conversion(void) {
    // Start the conversion
    ADC0.COMMAND |= ADC_STCONV_bm;
}

void ADC_Stop_Conversion(void) {
    // Stop the current conversion
    ADC0.COMMAND |= ADC_SPCONV_bm;
}

uint8_t ADC_Is_Conversion_Done(void)    {
    // Check for ADC Conv to be complete
    if(ADC0.INTFLAGS & ADC_RESRDY_bm)   {
        return 1;
    }
    return 0;
}

uint8_t ADC_Is_Window_Satisfied(void)  {
    if (ADC0.INTFLAGS & ADC_WCMP_bm)    {
        return 1;
    }
    return 0;
}


uint16_t ADC_Get_Result(void) {
    // Return the result also clears the RESRDY interrupt flag in ADC0 INTFLAGS register 
    return ADC0.RES;
}

void ADC_Start_Oversampled_Conversion(ADC_SAMPNUM_t sampnum) {
    // variable to store the bitshift 
    //uint8_t bitShift = 0;
    // disable ADC while we change the number of samples
    ADC_Disable();  
    // set up the required number of samples
    ADC0.CTRLB = (sampnum << ADC_SAMPNUM_gp);
    // turn ADC back on
    ADC_Enable();
    // accumulate required number of samples
    ADC_Start_Conversion();
}

uint16_t ADC_Get_Oversampled_Result(uint8_t enob)    {
    
    // get the set resolution from ADC0 CTRLA register and calculate diff between required enob and current ADC resolution setting (10 or 12 bit))
    // if ADC set to 12-bit result
    if (((ADC0.CTRLA & ADC_RESSEL_gm) >> 2) == 0x00)  {
        bitShift = enob - 12;
    }
    // if ADC set to 10-bit result
    else if (((ADC0.CTRLA & ADC_RESSEL_gm) >> 2) == 0x01)   {
        bitShift = enob - 10;
    }

    // do bitshift to get required resolution result 
    return (ADC0.RES >> bitShift);
}





