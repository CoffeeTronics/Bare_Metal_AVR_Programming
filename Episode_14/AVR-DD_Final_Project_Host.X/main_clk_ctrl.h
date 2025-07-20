// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MAIN_CLK_CTRL_H
#define	MAIN_CLK_CTRL_H

#include <xc.h> // include processor files - each processor file is guarded.  

void MainClkCtrl(CLKCTRL_CLKSEL_t, CLKCTRL_FRQSEL_t, char, CLKCTRL_PDIV_t, char);

void LowPowerClkCtrl(CLKCTRL_CLKSEL_t, char, CLKCTRL_PDIV_t, char, char);



#endif	/* MAIN_CLK_CTRL_H */

