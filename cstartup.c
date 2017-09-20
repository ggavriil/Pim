extern int __bss_start__;
extern int __bss_end__;

extern void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags);

void _cstartup(unsigned int r0, unsigned int r1, unsigned int r2) {
  /* bss contains statically allocated
   * variables. we need to initialise
   * them to 0 */
  int* bss_cur = &__bss_start__; //bss start and end defined in the linker
  int* bss_end = &__bss_end__;
  for(bss_cur; bss_cur < bss_end; bss_cur++) {
    *bss_cur = 0;
  }
  
  kernel_main(r0, r1, r2);

  //We should never return from main but
  //even if we do we will stay here
  while(1) {
    //Do nothing
  }
}
