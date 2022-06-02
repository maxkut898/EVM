//
// Created by evgenii on 08.03.17.
//
#include "draw_interface.h"


int Load_BIG_CHARS(void) {
    int file = open("BIG_CHARS", O_CREAT | O_RDONLY, S_IREAD);
    if (file == -1) {
        perror("Can't open file BIGCHAR\n");
        return -1;
    }

    size_t size = (size_t) (Num_BIG_CHAR * 2);
    memset(BigCharsArray, 0, size * sizeof(int32_t));
    lseek(file, 0, SEEK_SET);
    int count;
    int flg = bc_bigcharread(file, BigCharsArray, Num_BIG_CHAR, &count);
    if (flg != OK) {
        write(STDERR_FILENO, "ERR read BIGCHAR\n", strlen("ERR read BIGCHAR\n"));
        return -1;
    }
    close(file);
    return 0;
}

int fromArrayToBigChar(int32_t *big, char *arr) {
    int i;
    int x, y;

    for (i = 0; i < 64; i++) {
        x = i % 8;
        y = i / 8;
        bc_setbigcharpos(big, (uint8_t) y, (uint8_t) x, (uint8_t) arr[i]);
    }
    return 0;
}

int draw_interface(void) {
    mt_clrscr();
    uint16_t x_max, y_max;
    int flg = 0;
    flg = mt_getscreensize(&y_max, &x_max);
    if (flg != OK) {
        return ERR_SCREEN_SIZE;
    }
    if ((x_max < 82) || (y_max < 31)) {
        write(STDOUT_FILENO, "ERR_SCREEN_SIZE", strlen("ERR_SCREEN_SIZE"));
        return ERR_SCREEN_SIZE;
    }
    Load_BIG_CHARS();
    draw_operation(InctructionCounter);
    draw_memory(place_cell_memory.x, place_cell_memory.y);
    draw_accumulator();
    draw_instructionCounter();
    draw_flag();
    draw_bigchar(place_cell_memory.x * 10 + place_cell_memory.y);
    draw_keys();
    draw_IO();
    goto_console();
    return 0;
}

int draw_memory(uint x_curent, uint y_current) {
    int flg;
    mt_gotoXY(2, 2);
    for (int i = 0; i < 10; ++i) {
        uint x = (uint) (i * 10);
        for (int j = 0; j < 10; ++j) {
            uint y = (uint) j;
            char buf[10];
            int_least16_t value;
            sc_memoryGet(x + y, &value);
            print_memory_cell(buf, value);
            if ((i == x_curent) && (j == y_current)) {
                mt_setbgcolor(clr_green);
                mt_setfgcolor(clr_red);

            }
            if ((i == InctructionCounter / 10) && (j == InctructionCounter % 10)) {
                mt_setfgcolor(clr_blue);

            }
            write(STDOUT_FILENO, buf,
                  strlen(buf));
            if ((i == InctructionCounter / 10) && (j == InctructionCounter % 10)) {
                mt_setfgcolor(clr_default);
            }
            if ((i == x_curent) && (j == y_current)) {
                mt_setbgcolor(clr_default);
                mt_setfgcolor(clr_default);
            }
            write(STDOUT_FILENO, " ", 1);
        }
        write(STDOUT_FILENO, "\n ", strlen("\n "));
    }
    flg = bc_box(1, 1, 61, 12);
    if (flg != OK) {
        // return ERR_SCREEN_SIZE;
    }
    mt_gotoXY(29, 1);
    write(STDOUT_FILENO, "Memory", strlen("Memory"));
    goto_console();
    return OK;
}

int print_memory_cell(char *buf, int_least16_t cell) {
    if ((cell & (1 << 14)) != 0) {
        int_least16_t value;
        sc_valueDecode(&value, cell);
        if (value >= 0) {
            value ^= 1 << 14;
            sprintf(buf, "+%.4d", value);
        }
        else {
            value ^= 0 << 14;
            sprintf(buf, "%.4d", value);
        }
    }
    else {
        char comand;
        char operand;
        sc_commandDecode(cell, &comand, &operand);
        sprintf(buf, "%.2d:%.2d", comand, operand);
    }
    return OK;
}

int draw_accumulator(void) {
    bc_box(62, 1, 20, 3);
    mt_gotoXY(69, 2);
    char buf[6];
    print_memory_cell(buf, Accumulator);
    write(STDOUT_FILENO, buf, 6);
    mt_gotoXY(66, 1);
    write(STDOUT_FILENO, "Accumulator", strlen("Accumulator"));
    goto_console();
    return OK;
}

int draw_instructionCounter(void) {
    bc_box(62, 4, 20, 3);
    mt_gotoXY(69, 5);
    char buf[6];
    sprintf(buf, "+%.4d", InctructionCounter);
    write(STDOUT_FILENO, buf, 6);
    mt_gotoXY(63, 4);
    write(STDOUT_FILENO, "InctructionCounter", strlen("InctructionCounter"));
    goto_console();
    return OK;
}

int draw_operation(int address) {
    bc_box(62, 7, 20, 3);
    mt_gotoXY(69, 8);
    char buf[6];
    int_least16_t cell;
    sc_memoryGet(address, &cell);
    if(sc_it_command(cell)) {
        print_memory_cell(buf, cell);
    } else {
        print_memory_cell(buf, 0);
    }
    write(STDOUT_FILENO, buf, 6);
    mt_gotoXY(67, 7);
    write(STDOUT_FILENO, "Operation", strlen("Operation"));
    goto_console();
    return OK;
}

int draw_flag(void) {
    bc_box(62, 10, 20, 3);
    mt_gotoXY(69, 10);
    write(STDOUT_FILENO, "Flags", strlen("Flags"));
    char *flg[NUMBER_OF_FLAGS];
    flg[0] = "OV";
    flg[1] = "DBZ";
    flg[2] = "SF";
    flg[3] = "II";
    flg[4] = "WC";
    int value;
    char buf[15];
    memset(buf, '\0', 15);
    buf[0] = '-';
    for (int i = 0; i < NUMBER_OF_FLAGS; ++i) {
        size_t size = strlen(flg[i]);
        sc_regGet((uint_least8_t) pow(2, i), &value);
        if (value == 1) {
            strcat(buf, flg[i]);
            strcat(buf, "-");
        }
        else {
            char minus[size + 1];
            memset(minus, '-', size);
            minus[size] = '\0';
            strcat(buf, minus);
            strcat(buf, "-");
        }
    }
    mt_gotoXY(64, 11);
    write(STDOUT_FILENO, buf, strlen(buf));
    goto_console();
    return OK;
}

int draw_bigchar(int address) {
    int_least16_t big;
    sc_memoryGet(address, &big);
    bc_box(1, 13, 50, 10);
    char buf[6];
    print_memory_cell(buf, big);
    int32_t BigChar[2];
    for (int i = 0; i < strlen(buf); ++i) {
        switch (buf[i]) {
            case '0': {
                memcpy(BigChar, BigCharsArray, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case '1': {
                memcpy(BigChar, BigCharsArray + 2, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case '2': {
                memcpy(BigChar, BigCharsArray + 4, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case '3': {
                memcpy(BigChar, BigCharsArray + 6, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case '4': {
                memcpy(BigChar, BigCharsArray + 8, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case '5': {
                memcpy(BigChar, BigCharsArray + 10, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case '6': {
                memcpy(BigChar, BigCharsArray + 12, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case '7': {
                memcpy(BigChar, BigCharsArray + 14, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case '8': {
                memcpy(BigChar, BigCharsArray + 16, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case '9': {
                memcpy(BigChar, BigCharsArray + 18, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case 'a': {
                memcpy(BigChar, BigCharsArray + 20, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case 'b': {
                memcpy(BigChar, BigCharsArray + 22, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case 'c': {
                memcpy(BigChar, BigCharsArray + 24, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case 'd': {
                memcpy(BigChar, BigCharsArray + 26, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case 'e': {
                memcpy(BigChar, BigCharsArray + 28, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case 'f': {
                memcpy(BigChar, BigCharsArray + 30, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case '+': {
                memcpy(BigChar, BigCharsArray + 32, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case '-': {
                memcpy(BigChar, BigCharsArray + 34, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            case ':': {
                memcpy(BigChar, BigCharsArray + 36, sizeof(int32_t) * 2);
                bc_printbigchar(BigChar, (uint16_t) (2 + i * 10), 14, clr_default, clr_green);
                break;
            }
            default: {
                break;
            }
        }
    }
    goto_console();
    return OK;
}

int draw_keys(void) {
    bc_box(51, 13, 31, 10);
    mt_gotoXY(54,13);
    write(STDOUT_FILENO, "Keys:", strlen("Keys:"));
    mt_gotoXY(53, 14);
    write(STDOUT_FILENO, "l - load", strlen("l - load"));
    mt_gotoXY(53, 15);
    write(STDOUT_FILENO, "s - save", strlen("s - save"));
    mt_gotoXY(53, 16);
    write(STDOUT_FILENO, "r - run", strlen("r - run"));
    mt_gotoXY(53, 17);
    write(STDOUT_FILENO, "t - step", strlen("t - step"));
    mt_gotoXY(53, 18);
    write(STDOUT_FILENO, "i - reset", strlen("i - reset"));
    mt_gotoXY(53, 19);
    write(STDOUT_FILENO, "f5 - accumulator", strlen("f5 - accumulator"));
    mt_gotoXY(53, 20);
    write(STDOUT_FILENO, "f6 - instructionCounter", strlen("f5 - instructionCounter"));
    mt_gotoXY(53, 21);
    write(STDOUT_FILENO, "p - pause", strlen("p - pause"));
    goto_console();
    return 0;
}

int draw_IO(void) {
    bc_box(1, 23, 81, 8);
    mt_gotoXY(3, 23);
    write(STDOUT_FILENO, "I/O", strlen("I/O"));
    for (int i = 0; i < 6; ++i) {
        mt_gotoXY(2, (uint16_t) (24 + i));
        for (int j = 1; j < 79; ++j) {
            write(STDOUT_FILENO, " ", 1);
        }
        mt_gotoXY(2, (uint16_t) (24 + i));
        if (IO[i] != NULL) {
            write(STDOUT_FILENO, IO[i], strlen(IO[i]));
        }
    }
    goto_console();
    return 0;
}

int write_big_char() {
    char bigchar_plus[64] = {
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0
    };


    char bigchar_0[64] = {
            0, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 1, 1,
            1, 0, 0, 0, 0, 1, 0, 1,
            1, 0, 0, 0, 1, 0, 0, 1,
            1, 0, 0, 1, 0, 0, 0, 1,
            1, 0, 1, 0, 0, 0, 0, 1,
            1, 1, 0, 0, 0, 0, 0, 1,
            0, 1, 1, 1, 1, 1, 1, 0
    };

    char bigchar_1[64] = {
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 1, 1, 1, 0, 0, 0,
            0, 1, 0, 1, 1, 0, 0, 0,
            1, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    };

    char bigchar_2[64] = {
            0, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 0, 0, 0, 0, 1,
            0, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    };

    char bigchar_3[64] = {
            0, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            0, 1, 1, 1, 1, 1, 1, 0
    };

    char bigchar_4[64] = {
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 1, 0, 0,
            0, 1, 0, 0, 0, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0
    };

    char bigchar_5[64] = {
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            0, 1, 1, 1, 1, 1, 1, 0
    };

    char bigchar_6[64] = {
            0, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            0, 1, 1, 1, 1, 1, 1, 0
    };

    char bigchar_7[64] = {
            1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0
    };

    char bigchar_8[64] = {
            0, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            0, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            0, 1, 1, 1, 1, 1, 1, 0
    };

    char bigchar_9[64] = {
            0, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            0, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 0, 0, 0, 0, 1,
            0, 1, 1, 1, 1, 1, 1, 0
    };

    char bigchar_A[64] = {
            0, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1
    };

    char bigchar_B[64] = {
            1, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 1, 1, 1, 1, 1, 1, 0
    };

    char bigchar_C[64] = {
            0, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            0, 1, 1, 1, 1, 1, 1, 0
    };

    char bigchar_D[64] = {
            1, 1, 1, 1, 1, 0, 0, 0,
            1, 0, 0, 0, 0, 1, 0, 0,
            1, 0, 0, 0, 0, 0, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 1, 0,
            1, 0, 0, 0, 0, 1, 0, 0,
            1, 1, 1, 1, 1, 0, 0, 0
    };

    char bigchar_E[64] = {
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1
    };

    char bigchar_F[64] = {
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0
    };

    char bigchar_mins[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    };

    char bigchar_ddot[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    };
    int count = 19;
    size_t size = (size_t) (count * 2);
    int32_t array_big[size];
    memset(array_big, 0, size * sizeof(int32_t));

    fromArrayToBigChar(array_big, bigchar_0);
    fromArrayToBigChar(array_big + 2, bigchar_1);
    fromArrayToBigChar(array_big + 4, bigchar_2);
    fromArrayToBigChar(array_big + 6, bigchar_3);
    fromArrayToBigChar(array_big + 8, bigchar_4);
    fromArrayToBigChar(array_big + 10, bigchar_5);
    fromArrayToBigChar(array_big + 12, bigchar_6);
    fromArrayToBigChar(array_big + 14, bigchar_7);
    fromArrayToBigChar(array_big + 16, bigchar_8);
    fromArrayToBigChar(array_big + 18, bigchar_9);
    fromArrayToBigChar(array_big + 20, bigchar_A);
    fromArrayToBigChar(array_big + 22, bigchar_B);
    fromArrayToBigChar(array_big + 24, bigchar_C);
    fromArrayToBigChar(array_big + 26, bigchar_D);
    fromArrayToBigChar(array_big + 28, bigchar_E);
    fromArrayToBigChar(array_big + 30, bigchar_F);
    fromArrayToBigChar(array_big + 32, bigchar_plus);
    fromArrayToBigChar(array_big + 34, bigchar_mins);
    fromArrayToBigChar(array_big + 36, bigchar_ddot);
    int file = open("BIG_CHARS", O_CREAT | O_WRONLY, S_IWRITE);
    if (file == -1) {
        perror("Created ");
        return -1;
    }
    bc_bigcharwrite(file, array_big, count);
    close(file);
    return OK;
}

int goto_console() {
    if (place_pointer.replace_pointer)
        return 0;
    uint num = get_num_string_IO();
    mt_gotoXY(2, (uint16_t) (24 + num));
    return 0;
}

int draw_load_cell() {
    mt_setbgcolor(clr_cyan);
    mt_setfgcolor(clr_red);
    bc_box(20, 5, 23, 6);
    for (uint16_t i = 6; i < 10; ++i) {
        for (uint16_t j = 21; j < 42; ++j) {
            mt_gotoXY(j, i);
            write(STDOUT_FILENO, " ", 1);
        }
    }
    mt_gotoXY(22, 6);
    write(STDOUT_FILENO, "Enter Value", strlen("Enter value"));
    bc_box(21, 7, 21, 3);
    mt_gotoXY(22, 8);
    mt_setfgcolor(clr_default);
    mt_setbgcolor(clr_default);
    return 0;
}

int draw_load_save_memory() {
    mt_setbgcolor(clr_cyan);
    mt_setfgcolor(clr_red);
    bc_box(10, 5, 50, 6);
    for (uint16_t j = 6; j < 10; ++j) {
        for (uint16_t i = 11; i < 59; ++i) {
            mt_gotoXY(i, j);
            write(STDOUT_FILENO, " ", 1);
        }
    }
    mt_gotoXY(14, 6);
    write(STDOUT_FILENO, "Please specify the path to the memory file!", strlen("Please specify the path to the memory file!"));
    bc_box(15, 7, 40, 3);
    mt_gotoXY(16, 8);
    mt_setfgcolor(clr_default);
    mt_setbgcolor(clr_default);
    return 0;
}
