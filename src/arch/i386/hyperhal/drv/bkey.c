#include <arch/i386/hyperhal/drv/bkey.h>
#include <printf.h>
#include <arch/i386/hyperhal/asm/io.h>
#include <hyperhal/arch_hal/hal_interrupt.h>
#include <arch/i386/hyperhal/drv/bvideo.h>
#include <hyperhal/oops.h>

void keyboard_handler_int();

unsigned char keyboard_map[128] =
{
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',     /* 9 */
  '9', '0', '-', '=', '\b',     /* Backspace */
  '\t',                 /* Tab */
  'q', 'w', 'e', 'r',   /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
	0,                  /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',     /* 39 */
 '\'', '`',   0,                /* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',                    /* 49 */
  'm', ',', '.', '/',   0,                              /* Right shift */
  '*',
	0,  /* Alt */
  ' ',  /* Space bar */
	0,  /* Caps lock */
	0,  /* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,  /* < ... F10 */
	0,  /* 69 - Num lock*/
	0,  /* Scroll Lock */
	0,  /* Home key */
	0,  /* Up Arrow */
	0,  /* Page Up */
  '-',
	0,  /* Left Arrow */
	0,
	0,  /* Right Arrow */
  '+',
	0,  /* 79 - End key*/
	0,  /* Down Arrow */
	0,  /* Page Down */
	0,  /* Insert Key */
	0,  /* Delete Key */
	0,   0,   0,
	0,  /* F11 Key */
	0,  /* F12 Key */
	0,  /* All other keys are undefined */
};
unsigned char keyboard_map_upper[128] =
{
	0,  27, '!', '@', '#', '$', '%', '^', '&', '*',     /* 9 */
  '(', ')', '_', '+', '\b',     /* Backspace */
  '\t',                 /* Tab */
  'Q', 'W', 'E', 'R',   /* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
	0,                  /* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',     /* 39 */
 '\"', '`',   0,                /* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',                    /* 49 */
  'M', '<', '>', '?',   0,                              /* Right shift */
  '*',
	0,  /* Alt */
  ' ',  /* Space bar */
	0,  /* Caps lock */
	0,  /* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,  /* < ... F10 */
	0,  /* 69 - Num lock*/
	0,  /* Scroll Lock */
	0,  /* Home key */
	0,  /* Up Arrow */
	0,  /* Page Up */
  '-',
	0,  /* Left Arrow */
	0,
	0,  /* Right Arrow */
  '+',
	0,  /* 79 - End key*/
	0,  /* Down Arrow */
	0,  /* Page Down */
	0,  /* Insert Key */
	0,  /* Delete Key */
	0,   0,   0,
	0,  /* F11 Key */
	0,  /* F12 Key */
	0,  /* All other keys are undefined */
};

bool leftShiftPressed = false;
bool capslock_pressed = false;

char key0 = 0;
char key1 = 0;
char kbuff[256];

/*

Bit	Use
0	ScrollLock
1	NumberLock
2	CapsLock

Note: Other bits may be used in international keyboards for other purposes (e.g. a Japanese keyboard might use bit 4 for a "Kana mode" LED).

*/

typedef struct kleds_val
{
	uint8_t ScrollLock   : 1;
	uint8_t NumberLock   : 1;
	uint8_t CapsLock     : 1;
	uint8_t empty        : 1;
	uint8_t KanaMode     : 1;
	uint8_t empty1       : 3;
} kleds_val_t;

kleds_val_t kleds_global;

void i386_kbd_set_kleds(kleds_val_t val)
{
	uint8_t* kl = (uint8_t*)&val;
	write_port(0x64, 0xED);
	write_port(0x60, ((uint8_t*)kl)[0]);
	serial_printf("%s: "BYTE_TO_BINARY_PATTERN"\n", __FUNCTION__, BYTE_TO_BINARY(((uint8_t*)kl)[0]));
	serial_printf("%s: ScrollLock: %d NumberLock: %d CapsLock: %d KanaMode: %d\n", __func__, 
	kleds_global.ScrollLock, 
	kleds_global.NumberLock,
	kleds_global.CapsLock,
	kleds_global.KanaMode
	);
	serial_printf("%s: respond: %x-"BYTE_TO_BINARY_PATTERN"\n", __func__, read_port(0x64), BYTE_TO_BINARY(read_port(0x64)));
}
void i386_kb_init(void)
{
	/* This is a very basic keyboard initialization. The assumption is we have a
	 * PS/2 keyboard and it is already in a proper state. This may not be the case
	 * on real hardware. We simply enable the keyboard interupt */

	/* Get current master PIC interrupt mask */
	unsigned char curmask_master = read_port (0x21);

	/* 0xFD is 11111101 - enables only IRQ1 (keyboard) on master pic
	   by clearing bit 1. bit is clear for enabled and bit is set for disabled */
	write_port(0x21, curmask_master & 0xFD);

	// wipe kleds_global
	kleds_global.ScrollLock = 0;
	kleds_global.NumberLock = 0;
	kleds_global.CapsLock   = 0;
	kleds_global.empty      = 0;
	kleds_global.KanaMode   = 0;
	kleds_global.empty1     = 0;
	// i386_kbd_set_kleds(kleds_global);
}

char i386_getKey()
{
	while(1)
  	{
		asm("hlt\nhlt");
		if(key0){
		  	break;
		}
  	}
  	return key0;
}

char* i386_getKeyString()
{
	char input = 0;
	size_t c = 0;
	
	while(true)
	{
		input = i386_getKey();
		if(c > 256)
		{
			serial_printf("%s: BKey input buffer overflow!!!!!\n", __func__);
			return kbuff;
		}
		if(input == '\n')
		{
			kbuff[c] = 0;
			break;
		}
		if(input == '\b')
		{
			if(c == 0)
				kbuff[c] = 0;
			else
			{
				kbuff[c] = 0;
				c--;
			}
			continue;
		}
		if(input == 0)
			continue;
		else
		{
			kbuff[c] = input;
			c++;
			key1 = input;
			continue;
		}
	}
	return kbuff;
}

void keyboard_handler(void)
{
	signed char keycode;

	keycode = read_port(0x60);
	/* Only print characters on keydown event that have
	 * a non-zero mapping */
	if(keycode >= 0) {
		serial_printf("%s: pressed %x\n", __func__, keycode);
		if(keycode == 42)
		{
			leftShiftPressed = true;
		}
		if(keycode == 0x3A) // capslock
		{
			capslock_pressed = !capslock_pressed;
			kleds_global.CapsLock = !kleds_global.CapsLock;
			i386_kbd_set_kleds(kleds_global);
		}
		if(keycode == 0x5B) // win / super / command key
		{
			write_port(0x20, 0x20);
			serial_printf("%s: committing OOPS...\n", __func__);
			oops("Manually initiated OOPS");
		}
		if(keyboard_map[keycode]){
			if(leftShiftPressed | capslock_pressed)
			{
				printf("%c", keyboard_map_upper[keycode]);
				key0 = keyboard_map_upper[keycode];
			}
			else
			{
				printf("%c", keyboard_map[keycode]);
				key0 = keyboard_map[keycode];
			}
			leftShiftPressed = false;
		}
	}
	/* Send End of Interrupt (EOI) to master PIC */
	write_port(0x20, 0x20);
}

bool i386_kbd_init()
{
	hal_reg_interrupt(0x21, (uint32_t)keyboard_handler_int);
	i386_kb_init();
	return true;
}

void i386_reset_via_keyboard()
{
	bool if_ready = false;
	uint32_t data;
	while (if_ready) // wait while port is got empty
	{
		data = read_port(0x64);
		if (data == 0b00000010) if_ready = true;
	}
	write_port(0x64, 0xFE); // doing reset
}