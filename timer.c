#include "timer.h"

rpi_sys_timer_t* systemTimer = (rpi_sys_timer_t*) SYSTIMER_BASE;

void waitMicroSeconds(uint32_t us) {
  volatile uint32_t start = systemTimer->counter_lo;
  while(systemTimer->counter_lo - start < us) {
    //Do Nothing, just wait
  }
}
