#ifndef REGS_H
#define REGS_H
#include <types.h>

// Register structs for interrupt/exception
typedef struct regs32
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} regs32_t;

// Register structs for bios service
typedef struct regs16 {
    uint16_t di;
    uint16_t si;
    uint16_t bp;
    uint16_t sp;
    uint16_t bx;
    uint16_t dx;
    uint16_t cx;
    uint16_t ax;

    uint16_t ds;
    uint16_t es;
    uint16_t fs;
    uint16_t gs;
    uint16_t ss;
    uint16_t eflags;
} regs16_t;

// Standard clang interrupt frame structure
typedef struct interrupt_frame
{
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
} interrupt_frame_t;

#endif //  REGS_H