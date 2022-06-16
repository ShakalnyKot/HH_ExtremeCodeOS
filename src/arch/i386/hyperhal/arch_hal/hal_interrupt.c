#include <arch/i386/hyperhal/interrupt.h>
#include <hyperhal/arch_hal/hal_interrupt.h>

void hal_reg_einterrupt(hal_easy_interrupt_t* interrupt)
{
	i386_load_idt_entry(interrupt->vector, interrupt->handler, 0x08, 0x8e);
}

void hal_reg_interrupt(uint32_t vector, uint32_t handler)
{
	i386_load_idt_entry(vector, handler, 0x08, 0x8e);
}