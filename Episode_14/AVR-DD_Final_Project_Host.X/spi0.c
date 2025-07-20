
#include "spi0.h"

// number of SPI bytes to send to client device
// Total 2 bytes. This is made up of 2 bytes of ADC result (12-bit value right shifted), 
// where the MSb (Most Significant BIT) of the MSB (Most Significant BYTE) 
// contains the window result (1 = window satisfied & 0 = window NOT satisfied)
#define NUM_SPI_BYTES_TO_RECEIVE 2

// Initialize SPI Host device
void SPI_Host_Init(void)    {
    // use PA4, PA5, PA6, PA7 as MOSI, MISO, SCK, _SS respectively
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_DEFAULT_gc;    
    
    // MSB first, Set this MCU as SPI Master/Host, Divide main clock (4MHz) by 16 to get 250kHz SPI CLK 
    SPI0.CTRLA &= (~(SPI_DORD_bm) | ~(SPI_CLK2X_bm));
    SPI0.CTRLA = (SPI_MASTER_bm | (0x1 << SPI_PRESC_gp));  // SPI clk 250kHz
    
    // Use SPI Mode 0
    SPI0.CTRLB = SPI_MODE_0_gc;
    
    // Set MOSI (PA4), SCK (PA6), _SS (PA7) as outputs. Set MISO (PA5) as input
    PORTA.DIRSET = (PIN4_bm | PIN6_bm | PIN7_bm);    
    PORTA.DIRCLR = PIN5_bm;
    
    // Set _SS (PA7) high
    PORTA.OUTSET = PIN7_bm; 
    
    // SPI transfer Complete Interrupt Enable
    SPI0.INTCTRL |= SPI_TXCIE_bm;
    
    // Enable SPI peripheral
    SPI0.CTRLA |= SPI_ENABLE_bm;
}

// Select SPI Client device
void SPI_Select_Client(void)    {
    PORTA.OUTCLR = PIN7_bm;
}

// Deselect SPI Client device
void SPI_Deselect_Client(void)  {
    PORTA.OUTSET = PIN7_bm;
}

// Write a single SPI byte
void SPI_Write_Byte(uint8_t byte)   {
    SPI0.DATA = byte;
    while(!(SPI0.INTFLAGS & SPI_RXCIF_bm));
}

// Write a block of SPI data. Parameters are a pointer to the data block and the size of the block
void SPI0_WriteBlock(uint8_t *block, size_t size)   {
    uint8_t receive_data, *block_ptr;
    block_ptr = block;  // make a copy of *block so as to not modify it
    
    while (size--) {    // while we have more data
        SPI0.DATA = *block_ptr;                     // Write the data byte to the SPI DATA register
        while (!(SPI0.INTFLAGS & SPI_RXCIF_bm));    // Wait for Receive complete interrupt flag  
        receive_data = SPI0.DATA;                   // send same data back
        (void)(receive_data);                       // Silence compiler warning about receive_data set but not used
        block_ptr++;                                // increment our pointer to the data
    }
}

// Enable SPI peripheral
void SPI_Enable(void)   {
    SPI0.CTRLA |= SPI_ENABLE_bm;
}

// Disable SPI peripheral
void SPI_Disable(void)  {
    SPI0.CTRLA &= ~SPI_ENABLE_bm;
}

// Disable pins used in SPI bus
void SPI_Disable_Pins(void) {
    PORTA.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc | PORT_PULLUPEN_bm;
}

// Enable pins used in SPI bus
void SPI_Enable_Pins(void)  {
    PORTA.DIRSET = PIN4_bm | PIN6_bm | PIN7_bm;
    PORTA.DIRCLR = PIN5_bm;
}
