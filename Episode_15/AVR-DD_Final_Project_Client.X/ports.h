// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PORTS_H
#define	PORTS_H

#include <xc.h>

void PORT_Init(void);
void TurnOffUnusedPinsBeforeSleep(void);
uint8_t GetClientSelectFlagStatus(void);
void ClrClientSelectFlag(void);

#endif	/* PORTS_H */

