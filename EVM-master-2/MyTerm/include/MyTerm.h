//
// Created by jonn on 18.02.17.
//

#ifndef COMPUTER_MYTERM_H
#define COMPUTER_MYTERM_H

#include <stdint.h>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <memory.h>


#define OK                  0
#define ERR_PRINT           -7
#define ERR_IO_CONTROL      -8
#define ERR_SCREEN_SIZE     -9
#define ERR_SCREEN_COLOR    -10

#define CLRSCR "\E[H\E[2J"
#define GOTOXY "\E[%d;%dH"
#define FGCOLOR "\E[3%dm"
#define BGCOLOR "\E[4%dm"

typedef enum {
    clr_black,
    clr_red,
    clr_green,
    clr_brown,
    clr_blue,
    clr_magenta,
    clr_cyan,
    clr_light_blue,
    clr_default = 9
} Terminal_colors_t;

int mt_clrscr(void);
int mt_gotoXY(uint16_t x, uint16_t y);
int mt_getscreensize(uint16_t *rows, uint16_t *cols);
int mt_setfgcolor(Terminal_colors_t color);
int mt_setbgcolor(Terminal_colors_t color);

#endif //COMPUTER_MYTERM_H
