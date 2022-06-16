#include <arch/i386/hyperhal/drv/serial.h>
#include <printf.h>
#include <types.h>

char sbuff[256];

bool init_serial()
{
    return i386_init_serial();
}

int serial_received()
{
    return i386_serial_received();
}

char read_serial()
{
    return i386_read_serial();
}

int is_transmit_empty()
{
    return i386_is_transmit_empty();
}

void write_serial(char a)
{
    return i386_write_serial(a);
}

char* hal_serial_getstring()
{
    char key = 0;
    size_t counter = 0;
    for(int counter = 0;counter < 255; counter++)
    {
        sbuff[counter] = 0;
    }

    while(true)
    {
        key = read_serial();
        serial_printf("%c", key);
        if(counter > 256)
        {
	        serial_printf("%s: Serial input buffer overflow!!!!!\n", __func__);
	        return sbuff;
        }
        if(key == 0xD)
        {
	        serial_printf("\n");
	        break;
        }
        if(key == 0x8)
        {
	        if(counter == 0)
	        	sbuff[counter] = 0;
	        else
	        {
	        	sbuff[counter] = 0;
	        	counter--;
	        }
	        continue;
        }
        if(key == 0)
	        continue;
        else
        {
	        sbuff[counter] = key;
	        counter++;
	        continue;
        }
    }
    return sbuff;
}