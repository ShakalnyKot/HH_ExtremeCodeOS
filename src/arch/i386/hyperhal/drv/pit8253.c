#include <arch/i386/hyperhal/drv/pit8253.h>
#include <arch/i386/hyperhal/asm/io.h>
#include <arch/i386/hyperhal/drv/pic8259a.h>
#include <arch/i386/regs.h>
#include <printf.h>
#include <hyperhal/arch_hal/hal_interrupt.h>
//struct regs32;

// __attribute__((interrupt)) void timerTickHandler(struct regs32* frame) 
// { 
//     write_port(0x64, 0xED);
//     write_port(0x60, 0b1101);
//     printf("PIT8253_TICK %d!\r", __COUNTER__);
//     PIC_sendEOI(PIC_EOI);
// }

bool init_pit8253()
{
    set_clock_freq(IRQ_FREQ);
    //load_idt_entry(0x20, (unsigned long) timerTickHandler, 0x08, 0x8e);
    return true;
}

inline void set_clock_freq(uint32_t freq)
{
    uint16_t counter = (uint16_t)(CLOCK_FREQ / freq);
    write_port(PIT8253_MCR,
         PIT8253_MCR_BINARY | PIT8253_MCR_MODE2 | PIT8253_MCR_COUNTER0 | PIT8253_MCR_WRITE_LH);
    io_delay();
    write_port(PIT8253_COUNTER0, counter & 0xFF);
    io_delay();
    write_port(PIT8253_COUNTER0, (counter >> 8) & 0xFF);
    io_delay();
}

