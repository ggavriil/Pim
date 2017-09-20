.section ".text.startup"
.global _start
.global _get_stack_pointer

.equ    SCTLR_ENABLE_DATA_CACHE,        0x4
.equ    SCTLR_ENABLE_BRANCH_PREDICTION, 0x800
.equ    SCTLR_ENABLE_INSTRUCTION_CACHE, 0x1000


_start:
  // Initialise the stack pointer (grows
  // downwards)
  // ldr sp, =0x8000
  ldr sp, =0x4000000
  
  mrc p15, 0, r0, c1, c0, 2
  orr r0, r0, #0x300000            /* single precision */
  orr r0, r0, #0xC00000            /* double precision */
  mcr p15, 0, r0, c1, c0, 2
  mov r0, #0x40000000
  fmxr fpexc,r0
  
  // Enable L1 Cache -------------------------------------------------------

  // R0 = System Control Register
  mrc p15,0,r0,c1,c0,0

  // Enable caches and branch prediction
  orr r0,#SCTLR_ENABLE_BRANCH_PREDICTION
  orr r0,#SCTLR_ENABLE_DATA_CACHE
  orr r0,#SCTLR_ENABLE_INSTRUCTION_CACHE

  // System Control Register = R0
  mcr p15,0,r0,c1,c0,0


  //Run c startup - should not return
  //and will call kernel_main
  b _cstartup


.global dummy
dummy:
    bx lr


_inf_loop:
  b _inf_loop

_get_stack_pointer:
  //Return stack pointer value
  str sp, [sp]
  ldr r0, [sp]
  //Return 
  mov pc, lr


