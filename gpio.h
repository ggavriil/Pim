#define GPIO_BASE 0x20200000UL
#define CFG_GPIO_INP(N) *(gpio + (N/10)) &= ~(7<<((N % 10) * 3))
#define CFG_GPIO_OUT(N) *(gpio + (N/10)) |=  (1 << (N % 10) * 3)
#define SET_GPIO(N) *(gpio + 7)  = (1 << N)
#define CLR_GPIO(N) *(gpio + 10) = (1 << N)
#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH
#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock
 
