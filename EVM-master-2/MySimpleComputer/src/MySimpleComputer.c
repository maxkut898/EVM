#include "MySimpleComputer.h"

int sc_memoryInit(void) {
    for (u_long i = 0; i < MEMSIZE; ++i) {
        memory[i] = 0;
        memory[i] |= 1 << 14;
    }
    return OK;
}
int sc_memorySet(int address, int_least16_t value) {
    if (address < 0 && address > 99) {
        sc_regSet(FLAG_SEGMENTATION_FAULT, 1);
        return ERR_SEGMENTATION_FAULT;
    }
    memory[address] = value;
    return OK;
}

int sc_memoryGet(int address, int_least16_t *value) {
    if (address < 0 && address > 99) {
        sc_regSet(FLAG_SEGMENTATION_FAULT, 1);
        return ERR_SEGMENTATION_FAULT;
    }
    *value = memory[address];
    return OK;
}

int sc_memorySave(char *filename) {
    FILE *output;
    size_t size;

    output = fopen(filename, "wb");
    if (output == NULL)
        return ERR_OPEN_FILE;
    size = fwrite(&memory, sizeof(*memory), 100, output);
    fclose(output);
    if (size != 100)
        return ERR_FILE_SIZE;
    else
        return OK;
}

int sc_memoryLoad(char *filename) {
    FILE *input;
    size_t size;
    input = fopen(filename, "rb");
    if (input == NULL)
        return ERR_OPEN_FILE;
    size = fread(&memory, sizeof(*memory), 100, input);
    fclose(input);
    if (size > 100)
        return ERR_FILE_SIZE;
    else
        return OK;
}

int sc_regInit(void) {
    RegFlag = FLAG_OK;
    return OK;
}

int sc_regSet(uint_least8_t reg, int value) {
    u_char num = 0;
    for (u_char i = 0; i < NUMBER_OF_FLAGS; ++i) {
        if (pow(2, i) == reg) {
            num++;
        }
    }
    if (num == 0) {
        return ERR_INCORRECT_REGISTER_MASK;
    }
    if (value == 1) {
        RegFlag |= reg;
    }
    else if (value == 0) {
        RegFlag &= ~reg;
    }
    else {
        return ERR_INCORRECT_VALUE;
    }
    return OK;
}

int sc_regGet(uint_least8_t reg, int *value) {
    u_char num = 0;
    for (u_char i = 0; i < NUMBER_OF_FLAGS; ++i) {
        if (pow(2, i) == reg) {
            num++;
        }
    }
    if (num == 0) {
        return ERR_INCORRECT_REGISTER_MASK;
    }
    *value = (RegFlag & reg) > 0 ? 1 : 0;
    return OK;
}

int sc_commandEncode(char command, char operand, int_least16_t *value) {
    short tmp_command = 0b0000000000000000;
    tmp_command |= operand;
    tmp_command <<= 7;
    tmp_command |= command;
    tmp_command |= 0 << 14;
    *value = tmp_command;
    return OK;
}

int sc_commandDecode(int_least16_t value, char *command, char *operand) {
    if ((value & (1 << 14)) != 0) {
        sc_regSet(FLAG_WRONG_COMMAND, 1);
        return ERR_WRONG_COMMAND;
    }
    *command = (char)(value);
    *command &= ~(1 << 7);
    *operand = (char)(value >> 7);
    return OK;
}

int sc_valueEncode(int_least16_t value, int_least16_t *cell) {
    *cell = value;
    *cell |= 1 << 14;
    return OK;
}

int sc_valueDecode(int_least16_t *value, int_least16_t cell) {
    if ((cell & (1 << 14)) != 0) {
        *value = cell;
    }
    else {
        return ERR_WRONG_VALUE;
    }
    return OK;
}

bool sc_it_command(int_least16_t cell) {
    if ((cell & (1 << 14)) == 0) {
        return true;
    }
    return false;
}

bool sc_it_value(int_least16_t cell) {
    if ((cell & (1 << 14)) == 1) {
        return true;
    }
    return false;
}

int sc_AccumIniit(void) {
    Accumulator = 0x4000;
    return 0;
}

int sc_InstractionCounterInit(void) {
    InctructionCounter = 0;
    return 0;
}
