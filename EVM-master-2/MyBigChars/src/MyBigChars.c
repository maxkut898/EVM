//
// Created by evgenii on 25.02.17.
//
#include "MyBigChars.h"

int bc_printA(const char *str) {
    size_t len_buffer = strlen(ENTER_ALT_MODE) + strlen(str) + strlen(EXIT_ALT_MODE);
    char buffer[len_buffer + 1];
    strcpy(buffer, ENTER_ALT_MODE);
    strcat(buffer, str);
    strcat(buffer, EXIT_ALT_MODE);
    if (write(STDOUT_FILENO, buffer, strlen(buffer)) == -1) {
        return ERR_PRINT;
    }
    return OK;
}

int bc_box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    uint16_t rows, cols;
    int flg;
    flg = mt_getscreensize(&rows, &cols);
    if (flg == ERR_IO_CONTROL) {
        return ERR_IO_CONTROL;
    }

    if ((x1 < 0) || (x2 < 0) || (y1 < 0) || (y2 < 0) || ((x1 + x2) > cols) || ((y1 + y2) > rows)) {
        return ERR_LIMITS_OF_THE_SCREEN;
    }
    // X
    for(int i = 1; (x1 + i) < (x1 + x2) - 1; i++) {
        flg = mt_gotoXY((uint16_t) (x1 + i), y1);
        if (flg != OK) {
            return ERR_GOTOXY;
        }
        flg  = bc_printA(HORIZONTAL_LINE_CHAR);
        if (flg != OK) {
            return ERR_BC_PRINT;
        }
        flg = mt_gotoXY((uint16_t) (x1 + i), (uint16_t) (y1 + y2 - 1));
        if (flg != OK) {
            return ERR_GOTOXY;
        }
        flg = bc_printA(HORIZONTAL_LINE_CHAR);
        if (flg != OK) {
            return ERR_BC_PRINT;
        }
    }
    //
    // Y
    for(int i = 1; (y1 + i) < (y1 + y2) - 1; i++) {
        flg = mt_gotoXY(x1, (uint16_t) (y1 + i));
        if (flg != OK) {
            return ERR_GOTOXY;
        }
        flg = bc_printA(VERTICAL_LINE_CHAR);
        if (flg != OK) {
            return ERR_BC_PRINT;
        }
        flg = mt_gotoXY((uint16_t) ((x1 + x2) - 1), (uint16_t) (y1 + i));
        if (flg != OK) {
            return ERR_GOTOXY;
        }
        flg = bc_printA(VERTICAL_LINE_CHAR);
        if (flg != OK) {
            return ERR_BC_PRINT;
        }
    }
    //
    flg = mt_gotoXY(x1, y1);
    if (flg != OK) {
        return ERR_GOTOXY;
    }
    flg = bc_printA(TL_ANGLE_CHAR);
    if (flg != OK) {
        return ERR_BC_PRINT;
    }
    flg = mt_gotoXY((uint16_t) ((x1 + x2) - 1), y1);
    if (flg != OK) {
        return ERR_GOTOXY;
    }
    flg = bc_printA(TR_ANGLE_CHAR);
    if (flg != OK) {
        return ERR_BC_PRINT;
    }

    flg = mt_gotoXY(x1, (uint16_t) (y1 + y2 - 1));
    if (flg != OK) {
        return ERR_GOTOXY;
    }
    flg = bc_printA(DL_ANGLE_CHAR);
    if (flg != OK) {
        return ERR_BC_PRINT;
    }
    flg = mt_gotoXY((uint16_t) (x1 + x2 - 1), (uint16_t) (y1 + y2 - 1));
    if (flg != OK) {
        return ERR_GOTOXY;
    }
    flg = bc_printA(DR_ANGLE_CHAR);
    if (flg != OK) {
        return ERR_BC_PRINT;
    }
    return OK;
}

int bc_printbigchar(int32_t *in, uint16_t x, uint16_t y, Terminal_colors_t bg, Terminal_colors_t fg) {
    uint16_t x_max, y_max;
    int flg;
    flg = mt_getscreensize(&y_max, &x_max);
    if (flg != OK) {
        return ERR_SCREEN_SIZE;
    }
    uint8_t cols = 8;
    uint8_t rows = 8;

    if ((x < 0) || (y < 0) || ((x + cols) > x_max) || ((y + rows) > y_max)) {
        return ERR_LIMITS_OF_THE_SCREEN;
    }
    if ((fg < 0) || (fg > clr_default) || (bg < 0) || (bg > clr_default)) {
        return ERR_COLOR;
    }

    flg = mt_setbgcolor(bg);
    if (flg != OK) {
        return ERR_PRINT;
    }
    flg = mt_setfgcolor(fg);
    if (flg != OK) {
        return ERR_PRINT;
    }
    uint8_t value;
    for (uint16_t j = 0; j < rows; ++j) {
        flg = mt_gotoXY(x, (uint16_t) (y + j));
        if (flg != OK) {
            return ERR_GOTOXY;
        }
        for (uint16_t i = 0; i < cols; ++i) {
            bc_getbigcharpos(in, j, i, &value);
            flg = bc_printA((value == 1) ? (ACS_CKBOARD) : (" "));
            if (flg != OK) {
                return ERR_PRINT;
            }
        }
    }
    flg = mt_setbgcolor(clr_default);
    if (flg != OK) {
        return ERR_PRINT;
    }
    flg = mt_setfgcolor(clr_default);
    if (flg != OK) {
        return ERR_PRINT;
    }
    return OK;
}

int bc_setbigcharpos(int32_t *ch, uint16_t row, uint16_t col, uint8_t value)
{
    unsigned char tmp[8];
    memcpy(tmp, ch, sizeof(unsigned char) * 8);

    value == 1 ? (tmp[row] |= (1 << col)) : (tmp[row] &= ~(1 << col));

    memcpy(ch, tmp, sizeof(unsigned char) * 8);
    return OK;
}

int bc_getbigcharpos(int32_t *ch, uint16_t row, uint16_t col, uint8_t *value)
{
    unsigned char tmp[8];
    memcpy(tmp, ch, sizeof(unsigned char) * 8);

    *value = (uint8_t) ((tmp[row] & (1 << col)) == 0 ? 0 : 1);
    return OK;
}

int bc_bigcharwrite(int fd, int32_t *big, int count) {
    int size;

    size = (int) write(fd, &count, sizeof(int));
    if (size == -1)
        return ERR_WRITE;
    size = (int) write(fd, big, count * (sizeof(int32_t)) * 2);
    if (size == -1)
        return ERR_WRITE;
    return OK;
}

int bc_bigcharread(int fd, int32_t *big, int need_count, int *count) {
    int n, size, err;

    err = (int) read(fd, &n, sizeof(int));
    if (err == -1)
        return ERR_READ;
    size = (int) read(fd, big, need_count * sizeof(int32_t) * 2);
    if (size == -1)
        return ERR_READ;
    *count = size / (sizeof(int) * 2);
    return OK;
}