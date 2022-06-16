#ifndef MEM_ASM_H
#define MEM_ASM_H
#include <types.h>

uint32_t read_cr0();
void write_cr0(uint32_t);
uint32_t read_cr3();
void write_cr3(uint32_t);
uint32_t read_cr2();

#endif //MEM_ASM_H