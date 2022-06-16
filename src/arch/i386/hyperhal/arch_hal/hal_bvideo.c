#include <types.h>
#include <arch/i386/hyperhal/drv/bvideo.h>

void init_bvideo(uint8_t fore_color, uint8_t back_color)
{
    i386_init_vga((uint16_t*)BVIDEOBUF_ADDR, fore_color, back_color);
}

void bvideo_clearscr()
{
    i386_bvideo_clearscr();
}

void hal_save_current_bvideo_state()
{
    i386_bvideo_save_current_bvideo_state();
}

void hal_restore_current_bvideo_state()
{
    i386_bvideo_restore_current_bvideo_state();
}

void bputc(char c)
{
    i386_bputc(c);
}

void bputs(char* str)
{
    i386_bputs(str);
}

void bputd(int num)
{
    bputd(num);
}

void putchar(int c) // printf dummy func
{
    i386_putchar(c);
}

void bvideo_chgcolor(uint8_t fore, uint8_t back)
{
    i386_chgColor(fore, back);
}