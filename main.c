#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "stm8s003.h"
#include "spi.h"
#include "uart.h"
#include "stm8_utility.h"


#define SET_BIT(data,bitvalue)  PD_ODR = 0xff;             \
                                if(data & bitvalue) {      \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  PD_ODR = 0;              \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                }                          \
                                else {                     \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  PD_ODR = 0;              \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                  __asm__("nop");          \
                                }

#define SET_BYTE(data)          SET_BIT(data, 128) \
                                SET_BIT(data, 64) \
                                SET_BIT(data, 32) \
                                SET_BIT(data, 16) \
                                SET_BIT(data, 8) \
                                SET_BIT(data, 4) \
                                SET_BIT(data, 2) \
                                SET_BIT(data, 1)


int putchar(int c) {
    uart_write(c);
    return 0;
}




void push_rgb (uint8_t red, uint8_t green, uint8_t blue) {
  SET_BYTE(green)
  SET_BYTE(red)
  SET_BYTE(blue)
}


uint8_t sin_vals[32] = {0,26,51,77,99,122,143,163,184,199,214,227,237,245,252,255,255,252,247,242,232,219,207,191,173,153,133,110,84,61,36,10};


int main () {
    CLK_CKDIVR = 0;//16mhz
    //enable_interrupts();

    SPI_init();

    PD_DDR |= _BV(4);
    PD_CR1 |= _BV(4);

    uint8_t step = 0;
    uint8_t dir = 0;


    uint8_t sin_val = 0;

    //Main loop
    while(1) {

      if(dir)
        sin_val = 127 + sin_vals[step]/2;
      else
        sin_val = 127 - sin_vals[step]/2;


      push_rgb(sin_val, 0, 0);
      push_rgb(0, sin_val, 0);
      push_rgb(0, 0, sin_val);

      push_rgb(0, 255 - sin_val, 0);
      push_rgb(255-sin_val, sin_val, 0);


      util_delay_milliseconds(25);

      if(++step >= 32) {
        step = 0;
        dir = !dir;
      }
    }
}
