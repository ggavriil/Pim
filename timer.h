#include <stdint.h>

#define SYSTIMER_BASE 0x20003000

#ifndef TIMERH
#define TIMERH
typedef struct {
  volatile uint32_t control_status;
  volatile uint32_t counter_lo;
  volatile uint32_t counter_hi;
  volatile uint32_t compare0;
  volatile uint32_t compare1;
  volatile uint32_t compare2;
  volatile uint32_t compare3;
} rpi_sys_timer_t;
#endif

void waitMicroSeconds(uint32_t us);
