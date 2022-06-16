#include <arch/i386/hyperhal/drv/bvideo.h>
#include <arch/i386/hyperhal/asm/io.h>
#include <arch/i386/hyperhal/drv/vcursor.h>
#include <arch/i386/hyperhal/drv/serial.h>
#include <arch/i386/hyperhal/mm/mem.h>
#include <printf.h>
#include <hyperhal/string.h>

/*
16 bit video buffer elements
8 bits(ah) higher : 
  lower 4 bits - fore color
  higher 4 bits - back color

8 bits(al) lower :
  8 bits : ASCII character to print
*/

vbuff_entry_t* bvbuf;
char g_fore_color = 0;
char g_back_color = 0;
int digit_ascii_codes[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
static unsigned char col = 0;
static unsigned char row = 0;

vbuff_entry_t buff_snapshot[VRAM_SIZE];
static unsigned char col_snapshot = 0;
static unsigned char row_snapshot = 0;
char g_fore_color_snapshot = 0;
char g_back_color_snapshot = 0;

vbuff_entry_t i386_vga_entry(unsigned char ch, uint8_t fore_color, uint8_t back_color) 
{
  	return (vbuff_entry_t){ ch, fore_color, back_color };
}

//clear video buffer array
void i386_clear_vga_buffer(vbuff_entry_t **buffer, uint8_t fore_color, uint8_t back_color)
{
  	uint32_t i;
  	for(i = 0; i < VRAM_SIZE; i++){
  	  (*buffer)[i] = i386_vga_entry(NULL, fore_color, back_color);
  	}
  	col = 0;
  	row = 0;
}

//initialize vga buffer
void i386_init_vga(uint16_t* addr, uint8_t fore_color, uint8_t back_color)
{
  	bvbuf = (vbuff_entry_t*)addr;
  	i386_clear_vga_buffer(&bvbuf, fore_color, back_color);
  	g_fore_color = fore_color;
  	g_back_color = back_color;
}

//install vga buffer to another address
void i386_remap_vga(uint16_t* addr)
{
  	bvbuf = (vbuff_entry_t*)addr;
}

/*
 * Clear screen
*/
void i386_bvideo_clearscr()
{
	i386_clear_vga_buffer(&bvbuf, g_fore_color, g_back_color);
}

/* 
 * put char on display
 * c - char that need to put
*/
void i386_bputc(char c)
{
	((vbuff_entry_t*)(bvbuf))[(row * MAX_COL) + col] = i386_vga_entry(c, g_fore_color, g_back_color);
}

void i386_newPage( void )
{
	vbuff_entry_t vga1[VRAM_SIZE];
	for(uint32_t i = 0; i <= VRAM_SIZE; i++){
		vga1[i] = ((vbuff_entry_t*)bvbuf)[i];
	}

	for(uint32_t j = 0; j <= VRAM_SIZE; j++){
		((vbuff_entry_t*)bvbuf)[j] = i386_vga_entry(NULL, g_fore_color, g_back_color);
	}

	uint32_t offset = MAX_ROW * 3 + 5;
	for(uint32_t s = 0; s <= VRAM_SIZE; s++){
		((vbuff_entry_t*)bvbuf)[s] = vga1[s + offset];
	}
	
	row = MAX_ROW - 1;
	for(col = 0; col < MAX_COL; col++){
		i386_bputc(' ');
	}
	row = MAX_ROW - 1;
	col = 0;
}

/* 
 * put string on display
 * str - string
*/
void i386_bputs(char* str)
{
	for(int i = strlen(str), j = 0; j < i; j++)
	{
		switch (str[j])
		{
		case '\r':
			col = 0;
			continue;
		case '\n':
			col = 0;
			row++;
			continue;
		default:
			if(col > MAX_COL){
				col = 0;
				row++;
			}
			if(row > MAX_ROW){
				row = 0;
				col = 0;
				i386_newPage();
			}
			i386_bputc(str[j]);
			col++;
			break;
		}
	}
}

uint32_t digit_count(int num)
{
	uint32_t count = 0;
	if(num == 0)
		return 1;
	while(num > 0){
		count++;
		num = num/10;
	}
	return count;
}

/* 
 * put number on display
 * num - number
*/
void i386_bputd(int num)
{
	char str_num[digit_count(num)+1];
	itoa(num, str_num);
	i386_bputs(str_num);
}

// Place a character on next screen position
static void i386_cons_putc(int c)
{
	switch (c) 
	{
	case '\t':
		do 
		{
			i386_cons_putc(' ');
		} while ((col % 8) != 0);
		break;
	case '\r':
		col = 0;
		break;
	case '\n':
		if(row >= MAX_ROW){
			row = 0;
			col = 0;
			i386_newPage();
		}
		break;
	case '\b':
		if (col > 0) 
		{
			col -= 1;
			i386_bputc(' ');
		}
		break;
	default:
		i386_bputc(c);
		col += 1;
		if (col >= MAX_COL) 
		{
			col = 0;
			row += 1;
			if (row >= MAX_ROW) 
			{
				row = 0;
			}
		}
	};
}

// dummy function for printf
void i386_putchar( int c )
{
	if (c == '\n'){
		i386_cons_putc('\r');
		row += 1;
	}
	i386_cons_putc(c);  
}

// change color
void i386_chgColor(uint8_t fore, uint8_t back)
{
	g_fore_color = fore;
	g_back_color = back;
}

void i386_bvideo_save_current_bvideo_state()
{
	for (uint32_t i = 0; i < VRAM_SIZE; i++)
	{
		buff_snapshot[i] = ((vbuff_entry_t*)bvbuf)[i];
	}
	col_snapshot = col;
	row_snapshot = row;
	g_fore_color_snapshot = g_fore_color;
	g_back_color_snapshot = g_back_color;
	serial_printf("%s: saved\n", __FUNCTION__);
}

void i386_bvideo_restore_current_bvideo_state()
{
    for (uint32_t i = 0; i < VRAM_SIZE; i++)
	{
		((vbuff_entry_t*)bvbuf)[i] = buff_snapshot[i];
	}
	col = col_snapshot;
	row = row_snapshot;
	g_fore_color = g_fore_color_snapshot;
	g_back_color = g_back_color_snapshot;
	serial_printf("%s: restored\n", __FUNCTION__);
}