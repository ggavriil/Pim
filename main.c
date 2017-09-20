#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"
#include "engine.h"
#include "graphics.h"

volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;

int kernel_main(unsigned int r0, unsigned int r1, unsigned int atags) {
  uart_init();
  printf("Initialised Game\r\n");
  uint16_t* mptr = init_frame_buffer();
  game_loop(mptr);

  while(1) {
    //Do Nothing, Never exit main
  }


}
