#ifndef SERIAL_H
#define	SERIAL_H

#include <types.h>

#define COM1_SePORT 0x3f8          // COM1

bool i386_init_serial();
int i386_serial_received();
char i386_read_serial();
int i386_is_transmit_empty();
void i386_write_serial(char a);

#endif