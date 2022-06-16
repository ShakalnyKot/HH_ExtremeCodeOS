#include <arch/i386/hyperhal/drv/bvideo.h>
#include <arch/i386/hyperhal/drv/vcursor.h>
#include <arch/i386/hyperhal/drv/pic8259a.h>
#include <arch/i386/hyperhal/drv/pit8253.h>
#include <arch/i386/hyperhal/drv/bkey.h>
#include <arch/i386/hyperhal/drv/serial.h>
#include <arch/i386/hyperhal/interrupt.h>
#include <hyperhal/hhsc.h>
#include <hyperhal/oops.h>
#include <types.h>
#include <printf.h>

void arch_init()
{
	kmsg("\n**%s**\n", __func__);
	print_ok(init_pit8253(), "init_pit8253()...");
	print_ok(init_pic8259a(), "init_pic8259a()...");
	print_ok(init_idt(), "init_idt()...");
	print_ok(i386_kbd_init(), "i386_kbd_init()...");
	print_ok(oops_isr_init(), "oops_isr_init()...");
	print_ok(init_hhsc(0x50), "init_hhsc()...");
	kmsg("**%s**\n\n", __func__);
}