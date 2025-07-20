
#include "main_clk_ctrl.h"

void MainClkCtrl(CLKCTRL_CLKSEL_t clksel, CLKCTRL_FRQSEL_t frqsel, char prescaler_enable, CLKCTRL_PDIV_t pdiv, char clkout)  {

    // Write to MCLKCTRLA the settings for Clock Select and Clock out
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, (clksel << CLKCTRL_CLKSEL_gp) | (clkout << CLKCTRL_CLKOUT_bp));
    
    // if using prescaler
    if(prescaler_enable == 1)   {
        // Write to MCLKCTRLB the settings for Prescaler Divider, and Prescaler Enable
        _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (pdiv << CLKCTRL_PDIV_gp) | (prescaler_enable << CLKCTRL_PEN_bp));
    }
    // if not using prescaler, clear the Prescaler Enable bit
    else    {
        _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (prescaler_enable << CLKCTRL_PEN_bp));
    }
    // If using Internal High Freq Oscillator, set the required frequency 
    if(clksel == CLKCTRL_CLKSEL_OSCHF_gc)   {
        _PROTECTED_WRITE(CLKCTRL.OSCHFCTRLA, frqsel);
    }

    // if using CLKOUT set AVR64DD32 Curiosity Nano pin PA7 (CLKOUT) to output
    if (clkout == 1)    {
        PORTA.DIRSET = PIN7_bm; 
    }
}

void LowPowerClkCtrl(CLKCTRL_CLKSEL_t clksel, char prescaler_enable, CLKCTRL_PDIV_t pdiv, char runstandby, char clkout)  {
    
    #define CLK_PER 32ul    // Both CLK_PER and TIMEBASE_VALUE are used for delay
    #define TIMEBASE_VALUE ((uint8_t) ceil(CLK_PER*0.000001))   

    // Write to MCLKCTRLA the settings for Clock Select and Clock out
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, ((clksel << CLKCTRL_CLKSEL_gp) | (clkout << CLKCTRL_CLKOUT_bp)));
    
    // Write to MCLKCTRLB the settings for Prescaler Divider, and Prescaler Enable
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, pdiv | (prescaler_enable << CLKCTRL_PEN_bp));
    
    // Write to OSC32KCTRLA the setting for Run Standby
    _PROTECTED_WRITE(CLKCTRL.OSC32KCTRLA, (runstandby << CLKCTRL_RUNSTBY_bp));
    
    // if using CLKOUT set AVR64DD32 Curiosity Nano pin PA7 (CLKOUT) to output
    if (clkout == 1)    {
        PORTA.DIRSET = PIN7_bm; 
    }
}



