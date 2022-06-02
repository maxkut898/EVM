#ifndef INC_MEMORY_LIBRARY_H
#define INC_MEMORY_LIBRARY_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <sys/types.h>
#include <stdbool.h>

#define MEMSIZE 100


#define OK                          0
#define ERR_SEGMENTATION_FAULT      -1
#define ERR_OPEN_FILE               -2
#define ERR_FILE_SIZE               -3
#define ERR_INCORRECT_VALUE         -4
#define ERR_INCORRECT_REGISTER_MASK -5
#define ERR_WRONG_COMMAND           -6

#define ERR_WRONG_VALUE             -7

#define NUMBER_OF_FLAGS         5

#define FLAG_OK                 0x00

#define FLAG_OVERFLOW           0x01
#define FLAG_DIVISION_BY_ZERO   0x02
#define FLAG_SEGMENTATION_FAULT 0x04
#define FLAG_IGNORE_IMP         0x08
#define FLAG_WRONG_COMMAND      0x10




static int_least16_t memory[MEMSIZE];

uint_least8_t RegFlag;
int_least16_t Accumulator;
uint_least16_t InctructionCounter;

int sc_memoryInit(void);
int sc_memorySet(int address, int_least16_t value);
int sc_memoryGet(int address, int_least16_t * value);
int sc_memorySave(char *filename);
int sc_memoryLoad(char *filename);
int sc_regInit(void);
int sc_regSet(uint_least8_t reg, int value);
int sc_regGet(uint_least8_t reg, int *value);
int sc_commandEncode(char command, char operand, int_least16_t *value);
int sc_commandDecode(int_least16_t value, char *command, char *operand);

int sc_valueEncode(int_least16_t value, int_least16_t *cell);
int sc_valueDecode(int_least16_t *value, int_least16_t cell);
bool sc_it_command(int_least16_t cell);
bool sc_it_value(int_least16_t cell);
int sc_AccumIniit(void);
int sc_InstractionCounterInit(void);

#endif