#ifndef IO_H
#define IO_H
#include <types.h>
#define INT_ON  asm("sti")
#define INT_OFF asm("cli")
#define HLT     asm("hlt")

// Reading port
uint32_t read_port(uint32_t port);

// Writing port
void write_port(uint32_t port, uint32_t data);

// idfk i dont know why this shit was here
void io_delay();

#endif //  IO_H