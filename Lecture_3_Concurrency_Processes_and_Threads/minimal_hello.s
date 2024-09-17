/*
 * minimal_hello.s - Minimal Assembly Program
 *
 * 1. Compile the program using gcc:
 *    gcc minimal_hello.s -c
 *
 * 2. Link the object file (output will default to `a.out`):
 *    ld minimal_hello.o
 *
 * 3. Run the program:
 *    ./a.out
 *
 * 4. Check the size of the generated output file:
 *    ls -l a.out
 *
 * This program prints "Hello, OS World" in red using ANSI color codes and then exits.
 */


#include <sys/syscall.h>
.globl _start 
_start:
    movq $1, %rax           # write syscall number (sys_write)
    movq $1, %rdi           # fd=1 (stdout)
    movq $st, %rsi          # buf=st (pointer to string)
    movq $(ed - st), %rdx   # count=ed-st (length of string)
    syscall                 # invoke syscall

    movq $60, %rax          # exit syscall number (sys_exit)
    movq $1, %rdi           # status=1 (exit status)
    syscall                 # invoke syscall

st:
    .ascii "\033[01;31mHello, OS World\033[0m\n"   # ANSI escape sequence for red text
ed:
