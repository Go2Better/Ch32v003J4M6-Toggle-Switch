Firmware for CH32V003J4M6 for toggle switch using Mounriver Studio 
and ch32v003-vanilla-gcc of  https://github.com/maxgerhardt/ch32v003-vanilla-gcc/tree/main

   ** I use Google Gemini  https://gemini.google.com  to created most this code. **

but I use libraries from  https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/tag/v12.2.0-3/

I got some problem with MCU delay time but not sure what made its 300s delay use more 10 minutes.
...Not sure of what code of 50MHz = 30MHz in ch32v00x_gpio.h
So I have adjusted value from 300s to 150s delay in my code for nearly 5 minuts delay in my project.

clip of using this code  https://www.facebook.com/share/v/1ARYg7bUG3/ 
