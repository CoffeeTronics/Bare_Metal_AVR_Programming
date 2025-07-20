/* 
 * File:   tca0.h
 * Author: C52467
 *
 * Created on August 6, 2024, 2:24 PM
 */

#ifndef TCA0_H
#define	TCA0_H

#include <xc.h>

#define TCA_PERIOD 500  // clk period = 250ns, using prescaler of 8 gives 2us. To get 1ms requires period of 500


void TCA0_Init(uint16_t);
void TCA0_Enable(void);
void TCA0_Disable(void);
//void TCA0_Set_Period(uint16_t period);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* TCA0_H */

