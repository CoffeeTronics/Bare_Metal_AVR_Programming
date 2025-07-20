/*
 * File:   main.c
 * Author: C52467
 *
 * Created on November 7, 2023, 9:55 AM
 */

#define F_CPU 4E6
#include <xc.h>
#include <util/delay.h>
#include "main_clk_ctrl.h"
#include "usart0_tx.h"
//#include <avr/ioavr64dd32.h>

uint8_t spi_data_send_byte = 0x00;
uint8_t spi_data_last_sent_byte = 0x00;
uint8_t spi_data_recv;  

// function prototypes
void SPI_Host_Init();
void SPI_Select_Client(void);
void SPI_Deselect_Client(void);
inline uint8_t SPI_Read_Byte(void);
inline void SPI_Write_Byte(uint8_t);
inline uint8_t SPI_Exchange_Byte(uint8_t);


int main(void) {
    // 4MHz Main CLK, no prescaler, no CLKOUT
    // parameters: Clock Selection, Freq select, Prescaler Enable, Prescaler Divider, Clock out
    MainClkCtrl(CLKCTRL_CLKSEL_OSCHF_gc, CLKCTRL_FRQSEL_4M_gc, 0, CLKCTRL_PDIV_2X_gc, 0);
    
    USART_Init(USART_CMODE_ASYNCHRONOUS_gc, USART_PMODE_DISABLED_gc, USART_SBMODE_1BIT_gc, USART_CHSIZE_8BIT_gc);
    SPI_Host_Init();
    printf("\n\nTesting SPI Host:\n");

    while(1)    {
        SPI_Select_Client();
        spi_data_recv = SPI_Exchange_Byte(spi_data_send_byte);  
        SPI_Deselect_Client();
        
        printf("Most recent byte sent: %d\n", spi_data_send_byte);
        printf("Byte Received: %d\n", spi_data_recv);       
                
        if (spi_data_last_sent_byte != spi_data_recv)   {
            printf("Bytes NOT the same!\n\n");
        }
        else printf("Bytes match!\n\n");
        
        // save for next time
        spi_data_last_sent_byte = spi_data_send_byte;   

        _delay_ms(1);  
        
        // Increment data
        spi_data_send_byte++;
    }
    
    return 0;
}

// SPI 1MHz CLK, Master mode, MODE 0 (Resting CLK = LOW, Sample data on Rising edge, Setup/Drive data out falling edge)
void SPI_Host_Init()   {
    // use PA4, PA5, PA6, PA7 as MOSI, MISO, SCK, _SS respectively
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_DEFAULT_gc;    
    
    // MSB first, Set this MCU as SPI Master/Host, Divide main clock (4MHz) by 4 to get 1MHz SPI CLK & Enable SPI peripheral
    SPI0.CTRLA = ((0 << SPI_DORD_bp) | (1 << SPI_MASTER_bp) | (0 << SPI_CLK2X_bp) | (0x0 << SPI_PRESC_gp));// | (SPI_ENABLE_bm);
    
    // Set MOSI (PA4), SCK (PA6), _SS (PA7) as outputs. Set MISO (PA5) as input
    PORTA.DIRSET = (PIN4_bm | PIN6_bm | PIN7_bm);    // SS is output from this host
    PORTA.DIRCLR = PIN5_bm;
    
    // Enable Buffer mode (1 Tx Buffer for Master/Host) and set to Mode 0 (Most common mode)
    SPI0.CTRLB = (1 << SPI_BUFEN_bp) | (0x0 << SPI_MODE_gp);
    
    // enable Tx and Rx interrupts in Buffer mode
    SPI0.INTCTRL |= (SPI_TXCIE_bm | SPI_RXCIE_bm);
    
    // enable peripheral
    SPI0.CTRLA |= SPI_ENABLE_bm;
}

void SPI_Select_Client(void)    {
    // pull _SS LOW to begin transmission
    PORTA.OUTCLR = PIN7_bm;
}

void SPI_Deselect_Client(void)  {
    // pull _SS HIGH to end transmission
    PORTA.OUTSET = PIN7_bm;
}

void SPI_Write_Byte(uint8_t byte)    {
    // Write byte to data register
    SPI0.DATA = byte;
        
    // check to see buffer is ready before transmitting next byte
    while(!(SPI0.INTFLAGS & SPI_TXCIF_bm));
}

uint8_t SPI_Read_Byte(void) {
    return SPI0.DATA;
}


uint8_t SPI_Exchange_Byte(uint8_t byte)    { 
    SPI_Write_Byte(byte);
    
    // wait for received data interrupt flag to be set from incoming data
    while(!(SPI0.INTFLAGS & SPI_RXCIF_bm));
    
    return SPI_Read_Byte();
}





