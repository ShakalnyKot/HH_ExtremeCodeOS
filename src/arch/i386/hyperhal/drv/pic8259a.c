#include <arch/i386/hyperhal/drv/pic8259a.h>
#include <arch/i386/hyperhal/asm/io.h>

bool init_pic8259a()
{
    /* ICW1 - begin initialization */
    write_port(PIC_1_CTRL, 0x11);
    write_port(PIC_2_CTRL, 0x11);

    /* ICW2 - remap offset address of idt_table */
    /*
    * In x86 protected mode, we have to remap the PICs beyond 0x20 because
    * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    */
    write_port(PIC_1_DATA, 0x20);
    write_port(PIC_2_DATA, 0x28);

    /* ICW3 - setup cascading */
    write_port(PIC_1_DATA, 0x00);
    write_port(PIC_2_DATA, 0x00);

    /* ICW4 - environment info */
    write_port(PIC_1_DATA, 0x01);
    write_port(PIC_2_DATA, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    write_port(0x21 , 0xff);
    write_port(0xA1 , 0xff);
    return true;
}
 
void PIC_sendEOI(unsigned char irq)
{
	if(irq >= 8)
		write_port(PIC_2_CTRL,PIC_EOI);
 
	write_port(PIC_1_CTRL,PIC_EOI);
}