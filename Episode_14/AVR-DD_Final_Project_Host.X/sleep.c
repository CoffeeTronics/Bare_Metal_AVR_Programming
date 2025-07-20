#include "sleep.h"

// Initialize Sleep controller with Sleep mode, and Sleep Enable
void SLEEP_Init(SLPCTRL_SMODE_t sleep_mode, uint8_t sleep_enable)   {
    PORTF.INTFLAGS |= PIN6_bm;    // clear any previous PORT interrupt flags
    SLPCTRL.CTRLA = 0x00;   // clear any previous settings
    SLPCTRL.CTRLA = ((sleep_mode) | (sleep_enable << SLPCTRL_SEN_bp));
    
    // See datasheet pg 569 (Minimizes power consumption in different power modes))
    _PROTECTED_WRITE(SLPCTRL.VREGCTRL, SLPCTRL_PMODE_AUTO_gc | SLPCTRL_HTLLEN_OFF_gc);
}
