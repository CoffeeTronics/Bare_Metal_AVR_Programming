[AVR Bare Metal Programming - Episode 10: ADC Part 1](https://www.youtube.com/watch?v=unJQpIVJj68&list=PLtQdQmNK_0DQ8KGcZ1BOPv-3RDPvtqJ1H&index=10)

Learn how to create a bare metal ADC driver to read an analog signal and determine whether its within a user defined window. 
Then print that ADC result to the terminal and whether the window condition was satisfied. We will also accumulate 16 samples 
to cancel out random noise and use oversampling and decimation to increase the effective resolution of the ADC from 12 to 14-bits. 

Additionally, we will convert the Clock Control bare metal code from the previous video into a library so it can be easily reused in other projects.

Relevant Links:  
[AVR121: Enhancing ADC resolution by oversampling](https://ww1.microchip.com/downloads/en/appnotes/doc8003.pdf)  