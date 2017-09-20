#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#include "uart.h"
#include "utils.h"

extern void dummy ( unsigned int );


void PUT32(unsigned int addr, unsigned int val) {
  volatile uint32_t* mem = (uint32_t*) addr;
  *mem = val;
}

unsigned int GET32(unsigned int addr) {
  volatile uint32_t* mem = (uint32_t*) addr;
  return *mem;
}


void uart_init(void) {
  unsigned int ra;
  PUT32(AUX_ENABLES,1);
  PUT32(AUX_MU_IER_REG,0);
  PUT32(AUX_MU_CNTL_REG,0);
  PUT32(AUX_MU_LCR_REG,3);
  PUT32(AUX_MU_MCR_REG,0);
  PUT32(AUX_MU_IER_REG,0);
  PUT32(AUX_MU_IIR_REG,0xC6);
  PUT32(AUX_MU_BAUD_REG,270);

  ra=GET32(GPFSEL1);
  ra&=~(7<<12);
  ra|=2<<12; 
  PUT32(GPFSEL1,ra);

  PUT32(GPPUD,0);
  for(ra=0;ra<150;ra++) dummy(ra);
  PUT32(GPPUDCLK0,(1<<14));
  for(ra=0;ra<150;ra++) dummy(ra);
  PUT32(GPPUDCLK0,0);
  PUT32(AUX_MU_CNTL_REG,2);
}

void uart_putc(char c) {
  while(1) {
    if(GET32(AUX_MU_LSR_REG)&0x20) break;
  }
  PUT32(AUX_MU_IO_REG,c);
}


void uart_puts(char* str) {
  unsigned int len = strlen(str);
  for(unsigned int i = 0; i < len; i++) {
    uart_putc(str[i]);
  }
}

void uart_puti(long long i) {
  if(i == 0) {
    uart_puts("0");
  }
  unsigned int mag = 0;
  int mi = i;
  if(mi < 0) {
    mi *= -1;
  }
  while(mi > 0) {
    mi /= 10;
    mag++;
  }
  char* str = malloc((mag + 3) * sizeof(char));
  unsigned int cur = 0;
  if(i < 0) {
    str[cur++] = '-';
    i *= -1;
  }
  cur = mag - 1 + cur;
  str[cur + 1] = '\0';
  while(i > 0) {
    str[cur--] = (i % 10) + '0';
    i /= 10;
  }
  uart_puts(str);
  free(str);
}

void uart_puth (uint32_t d) {
    unsigned int rb;
    unsigned int rc;
    rb=32;
    while(rb > 0)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
    }
    uart_putc(0x20);
}
