#define F_CPU 32768UL
#include <xc.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "main_clk_ctrl.h"
#include "ports.h"
#include "sleep.h"
#include "spi0.h"
#include "usart0_tx.h"

// number of SPI bytes received by client device
// Total 2 bytes. This is made up of 2 bytes of ADC result (12-bit value right shifted), 
// where the MSb (Most Significant BIT) of the MSB (Most Significant BYTE) 
// contains the window result (1 = window satisfied & 0 = window NOT satisfied)
#define NUM_SPI_BYTES_TO_RECEIVE 2

// Declared in spi0.c
extern uint8_t spi_data[NUM_SPI_BYTES_TO_RECEIVE];

// State machine declaration
typedef enum {
    INIT = 0,
    SWITCH_TO_HI_SPD_CLK,
    RECEIVE_SPI,
    SWITCH_TO_LOW_PWR_CLK,
    WRITE_TO_USART,
    SLEEP
} APP_STATES;
            
// Wrap the states in a struct 
typedef struct  {
    APP_STATES state;
} APP_DATA;   

int main(void) {
    
    // create instance of state machine and starting state
    APP_DATA app_data;
    app_data.state = INIT;   
     
    while(1)    {
        
        switch(app_data.state)  {
        case INIT:
            // Initialize pins and pin change interrupt
            PORT_Init();
            
            // Initialize Low-Power Clk, prescaler disabled, runstandby disabled, no CLKOUT
            LowPowerClkCtrl(CLKCTRL_CLKSEL_OSC32K_gc, 0, CLKCTRL_PDIV_2X_gc, 0, 0);

            // Initialize USART for No Parity, 1 Stop Bit, 8 Data Bits, BAUD rate 1200 bps
            USART_Init(USART_CMODE_ASYNCHRONOUS_gc, USART_PMODE_DISABLED_gc, USART_SBMODE_1BIT_gc, USART_CHSIZE_8BIT_gc);

            // Initialize SPI as client device
            SPI_Client_Init();            

            // Initialize Sleep Controller for Power-Down mode and enable Sleep Mode
            SLEEP_Init(SLPCTRL_SMODE_PDOWN_gc, SLPCTRL_SEN_bm);
                   
            // Enable global interrupts
            sei();
            
            // Once initialized, go to sleep and wait for SPI data _SS interrupt to occur
            app_data.state = SLEEP;
            
            break;
            
            
        case SLEEP:
            asm("SLEEP");

            // Pin PA7 is the SPI Client Select (_SS). 
            // Check whether the software flag client_select_pulled_low_flag has been set by the ISR
            if (GetClientSelectFlagStatus() == 1)   {
                ClrClientSelectFlag();  // reset the software flag for next time
                app_data.state = SWITCH_TO_HI_SPD_CLK;  // change state to SWITCH_TO_HI_SPD_CLK
            }
            break;
           

        case SWITCH_TO_HI_SPD_CLK:
            // Use 4MHz Main Clk 
            // NOTE: CLKOUT is on pin PA7, which is also used for SPI _SS pin. Thus CLKOUT is not used.
            MainClkCtrl(CLKCTRL_CLKSEL_OSCHF_gc, CLKCTRL_FRQSEL_4M_gc, 0, CLKCTRL_PDIV_2X_gc, 0);
            app_data.state = RECEIVE_SPI;   // change state to RECEIVE_SPI
            break;
                
            
        case RECEIVE_SPI:
            
            while(!(Get_packet_complete_status()));     // wait until SPI packet complete
            
            Clr_packet_complete_status();               // clear software flag for next time

            app_data.state = SWITCH_TO_LOW_PWR_CLK;     // change state to SWITCH_TO_LOW_PWR_CLK   
                
            break;
            
            
        case SWITCH_TO_LOW_PWR_CLK:
            
            // initialize Low-Power Clk, prescaler disabled, runstandby disabled, no CLKOUT
            LowPowerClkCtrl(CLKCTRL_CLKSEL_OSC32K_gc, 0, CLKCTRL_PDIV_2X_gc, 0, 0);
            
            app_data.state = WRITE_TO_USART;    // change state to WRITE_TO_USART
            
            break;
                
            
        case WRITE_TO_USART:
            // Print SPI data to USART
            printf("\nSPI data: 0x%x, 0x%x\n\n", spi_data[1], spi_data[0]);
            // Place 2 SPI bytes into a uint16_t variable
            uint16_t results = (uint16_t)(spi_data[1] << 8) | (uint16_t)(spi_data[0]);
            // Print the full received packet as a uint16_t
            printf("Results: %u\n", results);
            // Isolate the MSb of the 2nd byte to obtain Window Comparison result
            uint16_t window_result = results >> 15;
            // Isolate the ADC result from the Window Comparison result
            uint16_t adc_result = (results & 0xFFF);
            printf("\nWindow Result: %u\tADC Result:%u\n", window_result, adc_result);
            _delay_ms(50);  // Short delay to ensure printf completes before we go back to sleep
            app_data.state = SLEEP; // change state to SLEEP
            
            break;
        }
    }
    return 0;
}
