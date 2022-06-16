#pragma once

#include <types.h>

bool init_serial();
int serial_received();
char read_serial();
int is_transmit_empty();
void write_serial(char a);
char* hal_serial_getstring();