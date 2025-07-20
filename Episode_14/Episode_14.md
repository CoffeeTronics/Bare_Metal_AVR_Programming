[Intro to Bare Metal Programming - Episode 14: Final Project - Part 1](https://www.youtube.com/watch?v=qEXw9ykNdzA&list=PLtQdQmNK_0DQ8KGcZ1BOPv-3RDPvtqJ1H&index=14)

This is the first video (of two) for the final project in this series. The final project consists of 2 AVR-DD Curiosity Nano dev boards; 
one is the Host device, while the other is the Client device. The Host and Client are connected via an SPI data bus. Both devices initialize 
and then go into the lowest power sleep state. When the button on the Host device is pressed, it wakes from sleep and takes an ADC reading 
and a Window Comparison measurement. It then sends the data over the SPI bus and returns to sleep. When the Client device detects an 
arriving SPI packet, it wakes from sleep and converts the SPI data packet into a human-readable format. It then writes the data out to 
the USART and returns to sleep. The final project leverages libraries built up during the course of this series.

