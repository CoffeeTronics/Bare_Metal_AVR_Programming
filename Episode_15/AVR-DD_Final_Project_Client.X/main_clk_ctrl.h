// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MAIN_CLK_CTRL_H
#define	MAIN_CLK_CTRL_H

#include <xc.h> // include processor files - each processor file is guarded.  


// Note: For both functions, if Prescaler Enable is set to 0, parameter for Prescaler Divider (PDIV) will be ignored

// parameters: Clock Selection, Freq select, Prescaler Enable, Prescaler Divider, Clock out
void MainClkCtrl(CLKCTRL_CLKSEL_t, CLKCTRL_FRQSEL_t, char, CLKCTRL_PDIV_t, char);

// parameters Clock Selection, prescaler_enable, Prescaler Divider, runstandby, Clock out
void LowPowerClkCtrl(CLKCTRL_CLKSEL_t, char, CLKCTRL_PDIV_t, char, char);


#endif	/* MAIN_CLK_CTRL_H */

