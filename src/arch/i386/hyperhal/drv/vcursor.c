#include <arch/i386/hyperhal/asm/io.h>
#include <arch/i386/hyperhal/drv/bvideo.h>

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	write_port(0x3D4, 0x0A);
	write_port(0x3D5, (read_port(0x3D5) & 0xC0) | cursor_start);
 
	write_port(0x3D4, 0x0B);
	write_port(0x3D5, (read_port(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor()
{
	write_port(0x3D4, 0x0A);
	write_port(0x3D5, 0x20);
}

uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    write_port(0x3D4, 0x0F);
    pos |= read_port(0x3D5);
    write_port(0x3D4, 0x0E);
    pos |= ((uint16_t)read_port(0x3D5)) << 8;
    return pos;
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * MAX_COL + x;
 
	write_port(0x3D4, 0x0F);
	write_port(0x3D5, (uint8_t) (pos & 0xFF));
	write_port(0x3D4, 0x0E);
	write_port(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}