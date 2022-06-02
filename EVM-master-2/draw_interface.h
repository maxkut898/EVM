//
// Created by evgenii on 08.03.17.
//

#ifndef COMPUTER_DRAW_INTERFACE_H
#define COMPUTER_DRAW_INTERFACE_H

#include "MySimpleComputer.h"
#include "MyBigChars.h"
#include "MyTerm.h"
#include "IO_buffer.h"
#include <sys/stat.h>
#include <fcntl.h>

#define Num_BIG_CHAR 19
int32_t BigCharsArray[Num_BIG_CHAR * 2];


typedef struct {
    uint address;
    int replace_pointer;
} pointer;
pointer place_pointer;

typedef struct {
uint x;
uint y;
} call_memory;
call_memory place_cell_memory;

int Load_BIG_CHARS(void);
int fromArrayToBigChar(int32_t *big, char *arr);
int write_big_char();
int draw_interface(void);
int draw_memory(uint x_current, uint y_current);
int print_memory_cell(char *buf, int_least16_t cell);
int draw_accumulator(void);
int draw_instructionCounter(void);
int draw_operation(int address);
int draw_flag(void);
int draw_bigchar(int address);
int draw_keys(void);
int draw_IO(void);
int goto_console();
int draw_load_cell();
int draw_load_save_memory();
#endif //COMPUTER_DRAW_INTERFACE_H
