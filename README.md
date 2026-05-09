# Bare_Metal_AVR_Programming Episode Guide

This series covers bare metal AVR programming across two microcontroller families: the **AVR Tiny2 (ATtiny1627)** in Episodes 1–7, and the **AVR DD (AVR64DD32)** in Episodes 8–15. The series builds progressively — each episode's code lives in the corresponding `Episode_N` folder.

📺 Full playlist: [Intro to Bare Metal Programming with the AVR® Tiny2 and AVR DD](https://www.youtube.com/playlist?list=PLtQdQmNK_0DQ8KGcZ1BOPv-3RDPvtqJ1H)

---

## Part 1 — AVR Tiny2 (ATtiny1627)

### Episode 1 — How to Get Started
**Board:** ATtiny1627 Curiosity Nano

Introduces the concept of bare metal programming and sets up the toolchain. Covers how to add Device Family Packs to MPLAB® X IDE, navigate the datasheet from within the IDE, and use tech briefs (TB3262) and bit-field macros to write readable bare metal C code. Explains module and register structure, bit/bitfield naming conventions, Configuration Change Protection (CCP), and fuses.

---

### Episode 2 — Creating a New Project and Accessing Headers
**Board:** ATtiny1627 Curiosity Nano

Walks through creating a new bare metal project in MPLAB X IDE and creating a `main.c` file. Covers how to locate and use the device header file, understand bit masks and bit position macros, and navigate the group mask definitions that make register access readable.

---

### Episode 3 — Blinking an LED with I/O Ports and Bitmasks
**Board:** ATtiny1627 Curiosity Nano

First hands-on peripheral: configuring a GPIO pin as a digital output and toggling the onboard LED at a 1-second interval. Introduces PORT registers, the DIR and OUT bitfields, and a simple blocking delay function to produce the timing.

---

### Episode 4 — (Non)-Blocking Functions and Timers
**Board:** ATtiny1627 Curiosity Nano

Compares blocking vs. non-blocking software design. Uses the Timer/Counter type B (TCB) peripheral to implement a non-blocking LED blink — replacing the busy-wait delay with a hardware timer polling loop. Covers TCB configuration, interrupt overflow flags, and writing toggle logic that doesn't lock the CPU.

---

### Episode 5 — Interrupt-Driven Blinky and Sleep Modes
**Board:** ATtiny1627 Curiosity Nano

Moves the LED toggle into an interrupt service routine (ISR) triggered by the TCB overflow flag, freeing the CPU to enter a sleep mode between interrupts. Covers enabling global interrupts, writing an ISR, and configuring Idle and Standby sleep modes.

---

### Episode 6 — Low Power Measurements
**Board:** ATtiny1627 Curiosity Nano

Practical measurement episode: modifies the Curiosity Nano for current measurement (removing the power LED jumper) and uses the on-board power debugger to measure actual current consumption in active mode, idle sleep, and standby sleep. Results are compared against the datasheet's expected values.

---

### Episode 7 — Lowest Power Blinky
**Board:** ATtiny1627 Curiosity Nano

Pushes power consumption to its minimum by using the Real-Time Counter (RTC) Periodic Interval Timer (PIT) to wake the device from Power-Down sleep to blink the LED, then immediately return to sleep. Compares the resulting current consumption against the earlier blinky implementations.

---

## Part 2 — AVR DD (AVR64DD32)

*Episodes 8–15 switch to the AVR64DD32 Curiosity Nano. The AVR DD family adds a 24 MHz internal oscillator, DAC, multi-voltage I/O on Port C, and a richer peripheral set. The series uses these episodes to build a modular bare metal driver library, culminating in a multi-board final project.*

---

### Episode 8 — USART
**Board:** AVR64DD32 Curiosity Nano

First episode on the AVR DD platform. Sets up the Clock Controller to use the internal 24 MHz oscillator and verifies the output with a logic analyzer. Then configures the USART peripheral in asynchronous (UART) mode — covering baud rate calculation, BAUD register values, TX/RX pin mux, and transmitting characters over serial to a terminal.

---

### Episode 9 — Header and Source Files
**Board:** AVR64DD32 Curiosity Nano

Refactors the USART driver from Episode 8 into a proper `.h`/`.c` library structure. Covers include guards, separating function prototypes from implementations, using device header enums in custom function signatures, and building a reusable driver that can be included in future projects.

---

### Episode 10 — ADC (Part 1)
**Board:** AVR64DD32 Curiosity Nano

Introduces the 12-bit differential ADC on the AVR DD. Writes a bare metal ADC driver to read an analog signal and determine whether it falls within a user-defined window. Prints the ADC result over USART (using the driver from Episode 9) for real-time monitoring.

---

### Episode 11 — ADC (Part 2)
**Board:** AVR64DD32 Curiosity Nano

Extends the ADC driver into a full initialization function that exposes most of the ADC peripheral's configuration options: reference voltage selection, prescaler, sample accumulation, oversampling, PGA gain, and window comparator mode. Demonstrates how to structure a flexible driver API.

---

### Episode 12 — SPI
**Board:** AVR64DD32 Curiosity Nano

Configures the SPI peripheral in host (master) mode on the AVR DD. Covers SPI mode selection (CPOL/CPHA), clock prescaler, pin mux, chip select handling, and writing a bare metal SPI transmit/receive driver. Demonstrates communication with an external SPI device.

---

### Episode 13 — TWI / I2C
**Board:** AVR64DD32 Curiosity Nano

Configures the Two-Wire Interface (TWI) peripheral for I2C host-mode communication. Covers start/stop conditions, 7-bit addressing, ACK/NACK handling, read and write transactions, and building a reusable TWI driver following the same header/source pattern established in Episode 9.

---

### Episode 14 — Final Project (Part 1)
**Board:** Two AVR64DD32 Curiosity Nanos

First half of the capstone project. Uses two AVR DD Curiosity Nanos — one acting as a Host and one as a Client — connected over SPI or TWI. Sets up the project structure, hardware connections, and begins implementing the Host-side firmware that sends commands and reads back data from the Client board.

---

### Episode 15 — Final Project (Part 2)
**Board:** Two AVR64DD32 Curiosity Nanos

Completes the final project. Implements the Client-side firmware and integrates all peripherals covered in the series (USART, ADC, SPI/TWI). The finished system demonstrates a Host MCU commanding a remote Client MCU, reading sensor data back over the bus and reporting it over serial — tying the full driver library together in a real application.
