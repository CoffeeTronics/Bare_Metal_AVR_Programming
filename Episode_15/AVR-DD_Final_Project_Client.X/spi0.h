/* 
 * File:   spi0.h
 * Author: C52467
 *
 * Created on August 5, 2024, 3:23 PM
 */

#ifndef SPI0_H
#define	SPI0_H

#include <xc.h>

// function prototypes
void SPI_Client_Init(void);
void SPI_Write_Byte(uint8_t);   // parameter: byte to be written over SPI
void SPI0_WriteBlock(uint8_t*, size_t); // parameters: pointer* to data block, size of block

uint8_t Get_packet_complete_status(void);
void Clr_packet_complete_status(void);

#endif	/* SPI0_H */

