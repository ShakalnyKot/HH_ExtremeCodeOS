#ifndef OOPS_H
#define OOPS_H
#include <types.h>
#include <arch/i386/hyperhal/drv/pic8259a.h>
#include <arch/i386/regs.h>

void oops(const char* msg);
void panic(const char* msg);
bool oops_isr_init();

#endif // !OOPS_H