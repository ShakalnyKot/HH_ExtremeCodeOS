#pragma once
#include <types.h>

typedef struct hal_easy_interrupt
{
    uint32_t vector;
    uint32_t handler;
} hal_easy_interrupt_t;

void hal_reg_einterrupt(hal_easy_interrupt_t* interrupt);
void hal_reg_interrupt(uint32_t vector, uint32_t handler);