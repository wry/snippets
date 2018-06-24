.globl _start
.text
_start:
  mov $169,        %rax
  mov $0xfee1dead, %rdi
  mov $672274793,  %rsi
  mov $0x1234567,  %rdx
  mov $0x0,        %r10
  syscall
