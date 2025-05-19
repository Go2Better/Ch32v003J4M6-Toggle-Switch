Firmware for CH32V003J4M6 for toggle switch using Mounriver Studio 
and ch32v003-vanilla-gcc of https://github.com/maxgerhardt/ch32v003-vanilla-gcc/tree/main

but I use libraries from https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/tag/v12.2.0-3/
Found some problem with MCU system timer not sure what made its 300ms delay use more 10 minutes.
...Not sure of what code of 50MHz = 30MHz in ch32v00x_gpio.h
So I have adjust value from 300ms to 150ms delay in my code for 5 minuts delay.

