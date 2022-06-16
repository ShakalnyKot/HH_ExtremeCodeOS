#include <arch/i386/hyperhal/drv/bkey.h>

char hal_kbd_getkey()
{
	return i386_getKey();
}

char* hal_kbd_getstring()
{
	return i386_getKeyString();
}

bool hal_kbd_init()
{
	return i386_kbd_init();
}