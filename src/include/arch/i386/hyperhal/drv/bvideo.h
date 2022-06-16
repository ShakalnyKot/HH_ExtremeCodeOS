#ifndef BVIDEO_H
#define BVIDEO_H
#include <types.h>

#define BVIDEOBUF_ADDR 0xB8000
#define MAX_COL  80		// Maximum number of columns 
#define MAX_ROW  25		// Maximum number of rows 
#define VRAM_SIZE (MAX_COL*MAX_ROW)	// Size of screen, in short's 

enum bvcolors {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};

typedef struct vbuff_entry
{
	uint8_t symbol;
	uint8_t fore_color : 4;
	uint8_t back_color : 4;
} vbuff_entry_t;

vbuff_entry_t i386_vga_entry(unsigned char ch, uint8_t fore_color, uint8_t back_color);
void i386_clear_vga_buffer(vbuff_entry_t **buffer, uint8_t fore_color, uint8_t back_color);
void i386_init_vga(uint16_t* addr, uint8_t fore_color, uint8_t back_color);
void i386_remap_vga(uint16_t* addr);
void i386_bvideo_clearscr();
void i386_bputc(char c);
void i386_bputs(char* str);
void i386_bputd(int num);
void i386_putchar( int c );
void i386_chgColor(uint8_t fore, uint8_t back);
void i386_bvideo_save_current_bvideo_state();
void i386_bvideo_restore_current_bvideo_state();

#endif //  BVIDEO_H