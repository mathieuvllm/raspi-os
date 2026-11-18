.section ".text.boot"

.global _start

_start:
@ send 3/4 cores to halt
  mrc p15, #0, r1, c0, c0, #5
  and r1, r1, #3
  cmp r1, #0
  bne halt

@ HW loads kernel at 0x8000,
@ so we put the stack there and move downwards
  mov sp, #0x8000

@ fill BSS section with 0s, as required
@ by C runtime
  ldr r4, =__bss_start
  ldr r9, =__bss_end
  mov r5, #0
  mov r6, #0
  mov r7, #0
  mov r8, #0
  b       2f

1:
  stmia r4!, {r5-r8}

2:
  cmp r4, r9
  blo 1b

@ enter the kernel main function :)
  ldr r3, =kernel_main
  blx r3

@ loop forever
halt:
  wfe
  b halt
