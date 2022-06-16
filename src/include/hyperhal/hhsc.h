#ifndef HHSC_H // HyperHAL SysCall
#define HHSC_H
#include <hyperhal/arch_hal/hal_interrupt.h>
#include <types.h>
#include <msg.h>

bool init_hhsc(uint8_t vector);
void hhsc_print(const char* msg);
void hhsc_reg_interrupt_hander(uint32_t vector, uint32_t handler);
void hhsc_print_msg(message_t* msg);
void hhsc_send_msg(message_t* msg);

#endif // !HHSC_H