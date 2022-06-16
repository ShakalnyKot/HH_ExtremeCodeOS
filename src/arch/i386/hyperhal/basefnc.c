#include <arch/i386/hyperhal/asm/io.h>
#include <arch/i386/hyperhal/drv/bkey.h>
#include <types.h>
#include <printf.h>
#include <stdarg.h>

void do_halt()
{
	kmsg("%s()...", __func__);
	INT_OFF;
	while (true) HLT;
}

void do_reset()
{
	kmsg("%s()...", __func__);
	i386_reset_via_keyboard();
}