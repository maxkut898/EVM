//
// Created by evgenii on 25.02.17.
//

#ifndef COMPUTER_MYBIGCHARS_H
#define COMPUTER_MYBIGCHARS_H

#include "MyTerm.h"
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#define OK                          0
#define ERR_PRINT                   -7
#define ERR_LIMITS_OF_THE_SCREEN    -11
#define ERR_GOTOXY                  -12
#define ERR_BC_PRINT                -13
#define ERR_COLOR                   -14
#define ERR_READ                    -15
#define ERR_WRITE                   -16

#define ENTER_ALT_MODE "\E(0"
#define EXIT_ALT_MODE "\E(B"

#define HORIZONTAL_LINE_CHAR "q"
#define VERTICAL_LINE_CHAR "x"
#define DL_ANGLE_CHAR "m"
#define DR_ANGLE_CHAR "j"
#define TL_ANGLE_CHAR "l"
#define TR_ANGLE_CHAR "k"
#define ACS_CKBOARD "a"

int bc_printA (const char *str);
int bc_box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
int bc_printbigchar(int32_t *in, uint16_t x, uint16_t y, Terminal_colors_t bg, Terminal_colors_t fg);
int bc_setbigcharpos(int32_t *ch, uint16_t row, uint16_t col, uint8_t value);
int bc_getbigcharpos(int32_t *ch, uint16_t row, uint16_t col, uint8_t *value);
int bc_bigcharwrite(int fd, int32_t  *big, int count);
int bc_bigcharread(int fd, int32_t *big, int need_count, int *count);

#endif //COMPUTER_MYBIGCHARS_H
