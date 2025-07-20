//
//#include "adc.h"
//
//uint16_t adc_read_val = 0;
//uint8_t bitShift = 0;
//uint8_t enob = 0;
//
//void ADC_Init(VREF_REFSEL_t vref, uint8_t vref_alwayson, uint8_t runstdby, ADC_CONVMODE_t convmode, uint8_t leftadj, 
//        ADC_RESSEL_t ressel, uint8_t freerun, ADC_INITDLY_t initdly, ADC_SAMPNUM_t sampnum, ADC_PRESC_t presc, 
//        ADC_SAMPDLY_t sampdly, uint8_t samplen, ADC_MUXPOS_t muxpos, ADC_MUXNEG_t muxneg, ADC_WINCM_t wincm, uint16_t winlt, uint16_t winht) {
//    
//    VREF.ADC0REF = (vref << VREF_REFSEL_gp) | (vref_alwayson << VREF_ALWAYSON_bp);
//    
//    ADC0.CTRLA = (runstdby << ADC_RUNSTBY_bp) | (convmode << ADC_CONVMODE_bp) | (leftadj << ADC_LEFTADJ_bp) 
//            | (ressel << ADC_RESSEL_gp) | (freerun << ADC_FREERUN_bp);
//    
//    ADC0.CTRLB = (sampnum << ADC_SAMPNUM_gp);   
//    ADC0.CTRLC = (presc << ADC_PRESC_gp);   
//    ADC0.CTRLD = ((sampdly << ADC_SAMPDLY_gp) | (initdly << ADC_INITDLY_gp));   
//    ADC0.SAMPCTRL = samplen << ADC_SAMPLEN_gp;
//    if (convmode == ADC_CONVMODE_DIFF_gc)   {
//        ADC0.MUXNEG = (muxneg << ADC_MUXNEG_gp);
//    }
//    ADC0.MUXPOS = (muxpos << ADC_MUXPOS_gp);  
//    
//    // if Window Cmp mode being used
//    if (wincm != ADC_WINCM_NONE_gc) {
//        // set Window Compare high and low thresholds
//        ADC0.WINLT = winlt;
//        ADC0.WINHT = winht;
//        // set Window Compare mode 
//        ADC0.CTRLE = (wincm << ADC_WINCM_gp); 
//    }
//}
//
//void ADC_Enable(void) {
//    ADC0.CTRLA |= (1 << ADC_ENABLE_bp);    
//}
//
//void ADC_Disable(void) {
//    ADC0.CTRLA |= (0 << ADC_ENABLE_bp);  
//    //ADC0.CTRLA &= ~(1 << ADC_ENABLE_bp);  // alternative method of clearing the bit
//}
//
//void ADC_Start_Conversion(void) {
//    // Start the conversion
//    ADC0.COMMAND |= ADC_STCONV_bm;
//    //ADC0.COMMAND |= (1 << ADC_STCONV_bp);  // alternative method of setting the bit 
//}
//
//void ADC_Stop_Conversion(void) {
//    // Stop the current conversion
//    ADC0.COMMAND |= ADC_SPCONV_bm;
//    //ADC0.COMMAND |= (1 << ADC_SPCONV_bp);   // alternative method of setting the bit 
//}
//
//uint8_t ADC_Is_Conversion_Done(void)    {
//    // Check for ADC Conversion to be complete
//    if(ADC0.INTFLAGS & ADC_RESRDY_bm)   {
//        return 1;
//    }
//    return 0;
//}
//
//uint8_t ADC_Is_Window_Satisfied(void)  {
//    if (ADC0.INTFLAGS & ADC_WCMP_bm)    {
//        return 1;
//    }
//    return 0;
//}
//
//uint16_t ADC_Get_Result(void) {
//    // Return the result also clears the RESRDY interrupt flag in ADC0 INTFLAGS register 
//    return ADC0.RES;
//}

//void ADC_Start_Oversampled_Conversion(ADC_SAMPNUM_t sampnum) {
//    // disable ADC while we change the number of samples
//    ADC_Disable();  
//    // set up the required number of samples
//    ADC0.CTRLB = (sampnum << ADC_SAMPNUM_gp);
//    // turn ADC back on
//    ADC_Enable();
//    // accumulate required number of samples
//    ADC_Start_Conversion();
//}



//uint16_t ADC_Get_Oversampled_Result(uint8_t enob)    {
//    
//    // get the set resolution from ADC0 CTRLA register and calculate diff between required enob and current ADC resolution setting (10 or 12 bit))
//    // if ADC set to 12-bit result
//    if (((ADC0.CTRLA & ADC_RESSEL_gm) >> 2) == 0x00)  {
//        bitShift = enob - 12;
//    }
//    // if ADC set to 10-bit result
//    else if (((ADC0.CTRLA & ADC_RESSEL_gm) >> 2) == 0x01)   {
//        bitShift = enob - 10;
//    }
//
//    // do bitshift to get required resolution result 
//    return (ADC0.RES >> bitShift);
//}

