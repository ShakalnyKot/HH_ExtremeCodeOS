#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <types.h>

void i386_load_idt_entry(int isr_number, unsigned long base, short int selector, unsigned char flags);
bool init_idt();

#endif