// This is a guard condition so that contents of this file are not included
// more than once. 
#ifndef SPI0_H
#define	SPI0_H

#include <xc.h>

// function prototypes
void SPI_Host_Init(void);
void SPI_Select_Client(void);
void SPI_Deselect_Client(void);
void SPI_Write_Byte(uint8_t);
void SPI0_WriteBlock(uint8_t*, size_t);
void SPI_Enable(void);
void SPI_Disable(void);
void SPI_Enable_Pins(void);
void SPI_Disable_Pins(void);

#endif	/* SPI0_H */

