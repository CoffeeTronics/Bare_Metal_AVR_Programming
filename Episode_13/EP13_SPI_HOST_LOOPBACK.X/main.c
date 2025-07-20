

#include <xc.h>
#include "usart0_tx.h"
#include "main_clk_ctrl.h"
//#include <util/delay.h>

uint8_t val = 0x00;

void SPI_Host_Init(void);
void SPI_Select_Client(void);
void SPI_Deselect_Client(void);
void SPI_Write_Byte(uint8_t);

void main(void) {
    
    MainClkCtrl(CLKCTRL_CLKSEL_OSCHF_gc, CLKCTRL_FRQSEL_4M_gc, 0, CLKCTRL_PDIV_2X_gc, 1);
    USART_Init(USART_CMODE_ASYNCHRONOUS_gc, USART_PMODE_DISABLED_gc, USART_SBMODE_1BIT_gc, USART_CHSIZE_8BIT_gc);
    
    SPI_Host_Init();
    
    while(1)    {
        printf("Testing SPI Host!\n");
        SPI_Select_Client();
        SPI_Write_Byte(val++);
        SPI_Deselect_Client();
    }
    return;
}

void SPI_Host_Init(void)    {
    // use PA4, PA5, PA6, PA7 as MOSI, MISO, SCK, _SS respectively
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_DEFAULT_gc;    
    
    // MSB first, Set this MCU as SPI Master/Host, Divide main clock (4MHz) by 4 to get 1MHz SPI CLK 
    SPI0.CTRLA &= (~(SPI_DORD_bm) | ~(SPI_CLK2X_bm));
    SPI0.CTRLA |= (SPI_MASTER_bm | (0x0 << SPI_PRESC_gp));
    
    // Set MOSI (PA4), SCK (PA6), _SS (PA7) as outputs. Set MISO (PA5) as input
    PORTA.DIRSET = (PIN4_bm | PIN6_bm | PIN7_bm);    
    PORTA.DIRCLR = PIN5_bm;
    
    SPI0.INTCTRL |= SPI_TXCIE_bm;
}

void SPI_Select_Client(void)    {
    PORTA.OUTCLR = PIN7_bm;
}

void SPI_Deselect_Client(void)  {
    PORTA.OUTSET = PIN7_bm;
}

void SPI_Write_Byte(uint8_t byte)   {
    SPI0.DATA = byte;
    while(!(SPI0.INTFLAGS & SPI_TXCIF_bm));
}


