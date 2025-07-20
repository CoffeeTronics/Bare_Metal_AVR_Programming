// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  

// function prototypes

// ADC_Init parameters: vref, vref_alwayson, runstdby, convmode, leftadj, ressel, freerun, initdly, 
// sampnum, presc, sampdly, muxpos, muxneg, window cmp, window upper accumulated threshold, window lower accumulated threshold    
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
void ADC_Enable_Power_Rails_Before_Conversion(void);
void ADC_Disable_Power_Rails_After_Conversion(void);


#endif	/* ADC_H */

