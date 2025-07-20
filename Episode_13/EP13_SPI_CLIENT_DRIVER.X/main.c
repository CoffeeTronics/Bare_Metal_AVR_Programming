
#include <xc.h>
#include "main_clk_ctrl.h"
#include "usart0_tx.h"
#include <avr/interrupt.h>

// function prototypes
void SPI_Client_Init();

// volatile variables to receive and write data
volatile uint8_t received_data = 0;      // used to store current received data
volatile uint8_t last_receive_data = 0; // used to store previous received data
volatile uint8_t receive_data_flag = 0; // software flag for received data


int main(void) {
    // 8MHz Main CLK, no prescaler, no CLKOUT
    MainClkCtrl(CLKCTRL_CLKSEL_OSCHF_gc, CLKCTRL_FRQSEL_8M_gc, 0, CLKCTRL_PDIV_2X_gc, 0);
    
    PORTF.DIRSET = PIN5_bm; // set LED as output
    
    USART_Init(USART_CMODE_ASYNCHRONOUS_gc, USART_PMODE_DISABLED_gc, USART_SBMODE_1BIT_gc, USART_CHSIZE_8BIT_gc);
   
    SPI_Client_Init();
    
    printf("SPI Client Test!\n");
    
    sei();
    
    while(1)    {

        if(receive_data_flag)   {
            printf("Rx: %d\n", received_data);
            receive_data_flag = 0;
        }
    }
    return 0;
}

// SPI Client only acts when host initiates
void SPI_Client_Init(void) {
    // Use PA4, PA5, PA6, PA7 as MOSI, MISO, SCK, _SS respectively
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_DEFAULT_gc;    
    
    // Set MOSI, SCK, _SS as inputs. Note: SS is input from the host to this client
    PORTA.DIRCLR = (PIN4_bm | PIN6_bm);     
    //Set MISO as output
    PORTA.DIRSET = PIN5_bm;
    //Set prescaler to 4 to match host (1MHz SPI CLK), set device as Client
    SPI0.CTRLA = SPI_PRESC_DIV4_gc & (~SPI_MASTER_bm);
    
    // Enable  buffer mode, use mode 0, enable buffer wait for receive
    SPI0.CTRLB = SPI_BUFEN_bm | SPI_MODE_0_gc | SPI_BUFWR_bm;
    
    SPI0.INTCTRL = SPI_RXCIE_bm;   

    SPI0.CTRLA |= SPI_ENABLE_bm;
}

ISR(SPI0_INT_vect)  {
    if (SPI0.INTFLAGS & SPI_RXCIF_bm)   {
        receive_data_flag = 1;      // set flag
        PORTF.OUTTGL = PIN5_bm;     // toggle LED
        received_data = SPI0.DATA;   // read received data into variable
        SPI0.DATA = received_data;     // write data back 
    }
}

