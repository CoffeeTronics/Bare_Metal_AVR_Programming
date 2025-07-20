
#include "adc.h"
#include <avr/interrupt.h>

// Volatile variable is set in ADC0_WCMP ISR
volatile uint8_t window_satisfied;

// If using one of the accumulation modes, user to set both the required bitshift and Effective Number of Bits (enob))
uint8_t bitShift = 0;
uint8_t enob = 0;

// Initialize ADC peripheral
void ADC_Init(VREF_REFSEL_t vref, uint8_t vref_alwayson, uint8_t runstdby, ADC_CONVMODE_t convmode, uint8_t leftadj, 
        ADC_RESSEL_t ressel, uint8_t freerun, ADC_INITDLY_t initdly, ADC_SAMPNUM_t sampnum, ADC_PRESC_t presc, 
        ADC_SAMPDLY_t sampdly, uint8_t samplen, ADC_MUXPOS_t muxpos, ADC_MUXNEG_t muxneg, ADC_WINCM_t wincm, uint16_t winlt, uint16_t winht) {
     
    VREF.ADC0REF = (vref << VREF_REFSEL_gp) | (vref_alwayson << VREF_ALWAYSON_bp);
    
    ADC0.CTRLA = (runstdby << ADC_RUNSTBY_bp) | (convmode << ADC_CONVMODE_bp) | (leftadj << ADC_LEFTADJ_bp) 
            | (ressel << ADC_RESSEL_gp) | (freerun << ADC_FREERUN_bp);
    
    ADC0.CTRLB = (sampnum << ADC_SAMPNUM_gp);   
    ADC0.CTRLC = (presc << ADC_PRESC_gp);   
    ADC0.CTRLD = ((sampdly << ADC_SAMPDLY_gp) | (initdly << ADC_INITDLY_gp));   
    ADC0.SAMPCTRL = samplen << ADC_SAMPLEN_gp;
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
        // Enable Window Comparison Interrupt
        ADC0.INTCTRL = ADC_WCMP_bm; 
    }
}

// Enable ADC peripheral
void ADC_Enable(void) {
    ADC0.CTRLA |= (1 << ADC_ENABLE_bp);    
}

// Disable ADC peripheral
void ADC_Disable(void) {
    ADC0.CTRLA &= ~(1 << ADC_ENABLE_bp);
}

// Start ADC conversion
void ADC_Start_Conversion(void) {
    ADC0.COMMAND |= ADC_STCONV_bm;
}

// Stop the current conversion
void ADC_Stop_Conversion(void) {
    ADC0.COMMAND |= ADC_SPCONV_bm;
}

// Check if ADC conversion is done
uint8_t ADC_Is_Conversion_Done(void)    {
    if(ADC0.INTFLAGS & ADC_RESRDY_bm)   {
        return 1;
    }
    return 0;
}

// Check if ADC Window threshold conditions are satisfied
uint8_t ADC_Is_Window_Satisfied(void)  {
    if ((ADC0.INTFLAGS & ADC_WCMP_bm)!= 0)    {
        return 1;
    }
    return 0;
}

// Start an oversampled ADC conversion
void ADC_Start_Oversampled_Conversion(ADC_SAMPNUM_t sampnum) {
    // disable ADC while we change the number of samples
    ADC_Disable();  
    // set up the required number of samples
    ADC0.CTRLB = (sampnum << ADC_SAMPNUM_gp);
    // turn ADC back on
    ADC_Enable();
    // accumulate required number of samples
    ADC_Start_Conversion();
}

// Get the ADC result. This also clears the RESRDY interrupt flag in ADC0 INTFLAGS register
uint16_t ADC_Get_Result(void) {
    return ADC0.RES;
}

uint16_t ADC_Get_Oversampled_Result(uint8_t enob)    {
    
    // get the current set resolution from ADC0 CTRLA register and calculate diff between required enob and current ADC resolution setting (10 or 12 bit))
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

// Turn ON power rails to analog sensor
void ADC_Enable_Power_Rails_Before_Conversion(void)    {
    // Pin PC3 as VDD
    PORTC.OUTSET = PIN3_bm;
    // Pin PC2 as GND
    PORTC.OUTCLR = PIN2_bm;
}

// Turn OFF power rails to analog sensor
void ADC_Disable_Power_Rails_After_Conversion(void) {
    // Disable inputs and activate internal pullups
    PORTC.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTC.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    
    // write pins HIGH
    PORTC.OUTSET |= (PIN2_bm | PIN3_bm);
}

// ADC Window Compare ISR
ISR(ADC0_WCMP_vect) {
    window_satisfied = 1;
    if((ADC0.INTFLAGS & ADC_WCMP_bm) == 1)  {
        ADC0.INTFLAGS |= ADC_WCMP_bm;   // clear the flag
    }
}

