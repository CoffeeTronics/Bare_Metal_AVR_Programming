#define F_CPU 32768
#include <xc.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "main_clk_ctrl.h"
#include "usart0_tx.h"
#include "spi0.h"
#include "adc.h"
#include "ports.h"
#include "sleep.h"


// number of SPI bytes to send to client device
// Total 2 bytes. This is made up of 2 bytes of ADC result (12-bit value right shifted), 
// where the MSb (Most Significant BIT) of the MSB (Most Significant BYTE) 
// contains the window result (1 = window satisfied & 0 = window NOT satisfied)
#define NUM_SPI_BYTES_TO_SEND 2

// These are used in the first byte in the SPI block
#define WINDOW_SATISFIED (1 << 7)       // if window satisfied, place a '1' in the MSb
#define WINDOW_NOT_SATISFIED (0 << 7)   // if window NOT satisfied, place a '0' in the MSb

uint8_t data_array[NUM_SPI_BYTES_TO_SEND];  // this array will contain the data sent over SPI

// This spin lock is used for delays when using the OSCHF high speed clock
// This is because the F_CPU macro has been defined for the 32.768kHz ULP Internal Oscillator
void Spin_Lock(uint16_t time_in_ms)    {
    // Use fixed point math to convert ms to us, including scaling to account for the for loop overhead
    uint16_t time_in_us = (time_in_ms * 10000UL) / 18UL;
    for (int i=0; i < time_in_us; i++)  {
        asm("NOP");
    }
}

// State machine declaration
typedef enum {
    INIT = 0,
    SWITCH_TO_HI_SPD_CLK,
    READ_ADC,
    SEND_SPI,
    SWITCH_TO_LOW_PWR_CLK,
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
            // set up low pwr clock for 32.768kHz internal oscillator
            LowPowerClkCtrl(CLKCTRL_CLKSEL_OSC32K_gc, 0, CLKCTRL_PDIV_6X_gc, 0, 0); 
            
            // initialize button switch, and SPI pins
            PORT_Init();    
    
            // initialize ADC for single-ended 12-bit conversion on pin PF2 (AIN18) with window comparison
            ADC_Init(VREF_REFSEL_VDD_gc, 0, 0, ADC_CONVMODE_SINGLEENDED_gc, 0, ADC_RESSEL_12BIT_gc, 
                    0, ADC_INITDLY_DLY16_gc, ADC_SAMPNUM_NONE_gc, ADC_PRESC_DIV2_gc, ADC_SAMPDLY_DLY0_gc, 
                    2, ADC_MUXPOS_AIN18_gc, ADC_MUXNEG_GND_gc, ADC_WINCM_INSIDE_gc, 1000, 3000);
            
            // Initialize USART for 1200 bps, no parity, 8 data bits, 1 stop bit
            USART_Init(USART_CMODE_ASYNCHRONOUS_gc, USART_PMODE_DISABLED_gc, USART_SBMODE_1BIT_gc, USART_CHSIZE_8BIT_gc);
            
            // initialize Sleep 
            SLEEP_Init(SLPCTRL_SMODE_PDOWN_gc, SLPCTRL_SEN_bm); 

            sei();  // enable global interrupts
            
            app_data.state = SLEEP; // move to SLEEP state
            break;
            
        case SLEEP:     
            asm("SLEEP");   // go to sleep
            
            if (GetButtonPressedStatus() == 1)  {   // if pin change ISR 
                ClrButtonPressedStatus();           // reset for next time
                app_data.state = READ_ADC;          // move to next state
            }
            break;
            
        case READ_ADC:
            ADC_Enable_Power_Rails_Before_Conversion(); // Turn on IO pins being used as power rails
            ADC_Enable();                               // Enable ADC peripheral
            _delay_ms(1);   // Allow time for VREF to settle before starting a conversion see datasheet pg 578 - VREF Startup time
            ADC_Start_Conversion();                     // Start ADC conversion
            while(!ADC_Is_Conversion_Done());           // Wait for conversion to complete
            
            data_array[0] = 0x00;    // clear array before writing new data
            data_array[1] = 0x00;
            
            // IMPORTANT: When ADC_Get_Result is read, it clears the Window Comparison interrupt flag
            // This is why the Window Comparison is read BEFORE getting the ADC result
            if(ADC_Is_Window_Satisfied() == 1)  {
                // Write Window Threshold result to MSB of data array 
                data_array[1] = WINDOW_SATISFIED;           // Indicates window conditions were satisfied
            }
            else {
                data_array[1] &= ~(WINDOW_NOT_SATISFIED);   // Indicates window conditions were NOT satisfied
            }
            uint16_t adc_result = ADC_Get_Result();         // Get the ADC result
            
            data_array[0] = (uint8_t)(adc_result & 0x00FF); // isolate low byte of ADC result and write to data_array[0]
            data_array[1] |= (uint8_t)((adc_result & 0xFF00) >> 8); // isolate high byte of ADC result and write to data_array[1] 

            ADC_Disable();                                  // Disable ADC to save power
            ADC_Disable_Power_Rails_After_Conversion();     // Disable ADC power rails to save power
            app_data.state = SWITCH_TO_HI_SPD_CLK;          // after reading ADC, move to next state
            break;
            
        case SWITCH_TO_HI_SPD_CLK:
            // Use 4MHz Main Clk 
            // NOTE: CLKOUT is on pin PA7, which is also used for SPI _SS pin
            MainClkCtrl(CLKCTRL_CLKSEL_OSCHF_gc, CLKCTRL_FRQSEL_4M_gc, 0, CLKCTRL_PDIV_2X_gc, 0);
            while(!(CLKCTRL.MCLKSTATUS & CLKCTRL_OSCHFS_bm)); // wait for OSCHF oscillator to be stable
            app_data.state = SEND_SPI;      // move to next state
            break;
            
        case SEND_SPI:
            SPI_Host_Init();        // Initialize host

            SPI_Select_Client();    // Select client device   
            
            Spin_Lock(4); // This 4ms spin lock will give the client a chance to respond to _SS interrupt before data is sent to it

            SPI0_WriteBlock(&data_array[0], NUM_SPI_BYTES_TO_SEND);     // write block of data to Client device
            
            SPI_Deselect_Client();  // Deselect client device once data block has been sent
            SPI_Disable();          // Disable SPI peripheral and SPI pins
            SPI_Disable_Pins();
            
            app_data.state = SWITCH_TO_LOW_PWR_CLK; // Move to next state
            break;
            
        case SWITCH_TO_LOW_PWR_CLK:
            // set up low pwr clock for 32.768kHz internal oscillator
            LowPowerClkCtrl(CLKCTRL_CLKSEL_OSC32K_gc, 0, CLKCTRL_PDIV_6X_gc, 0, 0);
            while(!(CLKCTRL.MCLKSTATUS & CLKCTRL_OSC32KS_bm)); // wait for OSC32K oscillator to be stable
            // Turn off SPI pins and LED pin
            TurnOffUnusedPinsBeforeSleep();
            // Go to sleep and wait for button press
            app_data.state = SLEEP;
        } 
    }
    return 0;
}




