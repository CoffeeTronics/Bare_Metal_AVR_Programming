
#include "spi0.h"
#include <avr/interrupt.h>

#define NUM_SPI_BYTES_TO_RECEIVE 2
// Array to hold received data from Host device
uint8_t spi_data[NUM_SPI_BYTES_TO_RECEIVE];

// volatile variables to work with incoming SPI data
volatile uint8_t data_index = 0;
volatile uint8_t packet_complete = 0;


// SPI Client only acts when host initiates
void SPI_Client_Init(void) {
    // Use PA4, PA5, PA6, PA7 as MOSI, MISO, SCK, _SS respectively
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_DEFAULT_gc;    
    
    //Set device as Client
    SPI0.CTRLA &= ~SPI_MASTER_bm;
    
    // Enable buffer mode, use mode 0, enable buffer wait for receive
    SPI0.CTRLB = SPI_MODE_0_gc | SPI_BUFWR_bm;  
    
    // SPI Receive Complete interrupt enable
    SPI0.INTCTRL = 0x00;
    SPI0.INTCTRL = SPI_IE_bm;
    SPI0.INTCTRL |= SPI_RXCIE_bm;   
    
    // Enable SPI peripheral
    SPI0.CTRLA |= SPI_ENABLE_bm;
}


// Getter function for determining whether SPI packet of 2 bytes is complete
uint8_t Get_packet_complete_status(void)   {
    return packet_complete;
}

// Clear function for determining whether SPI packet of 2 bytes is complete
void Clr_packet_complete_status(void)   {
    packet_complete = 0;
}

// ISR for SPI interrupt
ISR(SPI0_INT_vect)  {
    // If Receive Complete interrupt Flag has been set by incoming SPI byte
    if (SPI0_INTFLAGS & SPI_RXCIF_bm) {
        // Read the received byte into the spi_data array
        spi_data[data_index++] = SPI0.DATA;
        if (data_index == 2) {
            data_index = 0; // Reset index after receiving 2 bytes
            packet_complete = 1;
        }
    }
}


