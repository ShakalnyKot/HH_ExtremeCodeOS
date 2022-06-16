#pragma once

#include <types.h>

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
    WHITE
};

void init_bvideo(uint8_t fore_color, uint8_t back_color);
#ifdef HEADLESS
#define bvideo_clearscr()
#else
void bvideo_clearscr();
#endif
void bputc(char c);
void bputs(char* str);
void bputd(int num);
void putchar(int c); // printf dummy func
void bvideo_chgcolor(uint8_t fore, uint8_t back);
#ifndef HEADLESS
void hal_save_current_bvideo_state();
void hal_restore_current_bvideo_state();
#else
#define hal_save_current_bvideo_state()
#define hal_restore_current_bvideo_state()
#endif