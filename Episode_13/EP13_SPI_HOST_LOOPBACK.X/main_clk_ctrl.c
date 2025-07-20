//#include <avr/ioavr64dd32.h>

#include "main_clk_ctrl.h"

//void MainClkCtrl(CLKCTRL_CLKSEL_t clksel, CLKCTRL_FRQSEL_t frqsel, uint8_t prescaler_enable, CLKCTRL_PDIV_t pdiv, uint8_t clkout)  {
//void MainClkCtrl(CLKCTRL_CLKSEL_t clksel, CLKCTRL_FRQSEL_t frqsel, bool prescaler_enable, CLKCTRL_PDIV_t pdiv, bool clkout)  {
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



