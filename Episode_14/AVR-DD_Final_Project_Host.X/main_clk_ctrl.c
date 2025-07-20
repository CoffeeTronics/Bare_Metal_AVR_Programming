//#include <avr/ioavr64dd32.h>

#include "main_clk_ctrl.h"

// Configure Main Clock using Hi-Frq internal oscillator
void MainClkCtrl(CLKCTRL_CLKSEL_t clksel, CLKCTRL_FRQSEL_t frqsel, char prescaler_enable, CLKCTRL_PDIV_t pdiv, char clkout)  {
    
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, (clksel << CLKCTRL_CLKSEL_gp) | (clkout << CLKCTRL_CLKOUT_bp));
    
    // if using prescaler
    if(prescaler_enable == 1)   {
        _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (pdiv << CLKCTRL_PDIV_gp) | (prescaler_enable << CLKCTRL_PEN_bp));
    }
    // if not using prescaler
    else    {
        _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (prescaler_enable << CLKCTRL_PEN_bp));
    }
    
    if(clksel == CLKCTRL_CLKSEL_OSCHF_gc)   {
        _PROTECTED_WRITE(CLKCTRL.OSCHFCTRLA, frqsel);
    }

    // if using CLKOUT set AVR64DD32 Curiosity Nano pin PA7 (CLKOUT) to output
    if (clkout == 1)    {
        PORTA.DIRSET = PIN7_bm; 
    }
}

// Configure Low-Power Clock using 32.768kHz internal oscillator
void LowPowerClkCtrl(CLKCTRL_CLKSEL_t clksel, char prescaler_enable, CLKCTRL_PDIV_t pdiv, char runstandby, char clkout)  {

    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, ((clksel << CLKCTRL_CLKSEL_gp) | (clkout << CLKCTRL_CLKOUT_bp)));
    
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, pdiv | (prescaler_enable << CLKCTRL_PEN_bp));
    
    _PROTECTED_WRITE(CLKCTRL.OSC32KCTRLA, (runstandby << CLKCTRL_RUNSTBY_bp));
    
    // if using CLKOUT set AVR64DD32 Curiosity Nano pin PA7 (CLKOUT) to output
    if (clkout == 1)    {
        PORTA.DIRSET = PIN7_bm; 
    }
}



