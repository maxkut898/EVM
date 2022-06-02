//
// Created by jonn on 18.02.17.
//
#include "MyTerm.h"

int mt_clrscr(void) {
    if (write(STDOUT_FILENO, CLRSCR, strlen(CLRSCR)) == -1) {
        return ERR_PRINT;
    }
    return OK;
}

int mt_gotoXY(uint16_t x, uint16_t y) {
    char buf[40];
    char *gotoxy = GOTOXY;
    uint16_t rows, cols;
    int flg = mt_getscreensize(&rows, &cols);
    if (flg == ERR_IO_CONTROL) {
        return ERR_IO_CONTROL;
    }
    if ((x >= 0) && (y >= 0) && (x < cols) && (y < rows)) {
        if (sprintf(buf, gotoxy, y, x) > 0) {
            if (write(STDOUT_FILENO, buf, strlen(buf)) == -1) {
                return ERR_PRINT;
            }
        }
        else {
            return ERR_PRINT;
        }
    }
    else {
        return ERR_SCREEN_SIZE;
    }
    return OK;
}

int mt_getscreensize(uint16_t *rows, uint16_t *cols) {
    struct winsize window_size;
    if (!ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size)) {
        *rows = window_size.ws_row;
        *cols = window_size.ws_col;
    }
    else {
        return ERR_IO_CONTROL;
    }
    return OK;
}

int mt_setfgcolor(Terminal_colors_t color) {
    if(color < clr_black || color > clr_default){
        return ERR_SCREEN_COLOR;
    }
    char buf[15];
    if (sprintf(buf, FGCOLOR, color) > 0) {
        if (write(STDOUT_FILENO, buf, strlen(buf)) == -1) {
            return ERR_PRINT;
        }
    }
    else {
        return ERR_PRINT;
    }
    return OK;
}

int mt_setbgcolor(Terminal_colors_t color) {
    if(color < clr_black || color > clr_default){
        return ERR_SCREEN_COLOR;
    }
    char buf[15];
    if (sprintf(buf, BGCOLOR, color) > 0) {
        if (write(STDOUT_FILENO, buf, strlen(buf)) == -1) {
            return ERR_PRINT;
        }
    }
    else {
        return ERR_PRINT;
    }
    return OK;
}
