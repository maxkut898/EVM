//
// Created by evgenii on 21.04.17.
//

#include "cpu.h"

void table_command_init() {
    for (int i = 0; i < 100; ++i) {
        table_command[i] = NULL;
        for_alu[i] = false;
    }
    table_command[10] = READ;
    for_alu[10] = false;
    table_command[11] = WRITE;
    for_alu[11] = false;
    table_command[20] = LOAD;
    for_alu[20] = false;
    table_command[21] = STORE;
    for_alu[21] = false;
    table_command[30] = ADD;
    for_alu[30] = true;
    table_command[31] = SUB;
    for_alu[31] = true;
    table_command[32] = DIVIDE;
    for_alu[32] = true;
    table_command[33] = MUL;
    for_alu[33] = true;
    table_command[40] = JUMP;
    for_alu[40] = false;
    table_command[41] = JNEG;
    for_alu[41] = false;
    table_command[42] = JZ;
    for_alu[42] = false;
    table_command[43] = HALT;
    for_alu[43] = false;
    table_command[51] = NOT;
    for_alu[51] = true;
    table_command[52] = AND;
    for_alu[52] = true;
}

int alu(int command, int operand) {
    handler_command handler = table_command[command];
    int flg = 0;
    flg = (int)(intptr_t) handler((void *) (intptr_t) operand);
    return flg;
}


int cu() {
    char command, operand;
    int_least16_t memory_cell;
    int flg;
    flg = sc_memoryGet(InctructionCounter, &memory_cell);
    if (flg  !=  OK) {
        return -1;
    }
    flg = sc_commandDecode(memory_cell, &command, &operand);
    if (flg != OK) {
        sc_regSet(FLAG_WRONG_COMMAND, 1);
        sc_regSet(FLAG_IGNORE_IMP, 1);
        alarm(0);
        return -1;
    }
    if (command < 0 && command > 99)
        return -1;
    if(for_alu[command]) {
        flg = alu(command, operand);
        if (flg != OK)
            HALT(0);
    }
    else {
        handler_command handler = table_command[command];
        flg = (int)(intptr_t) handler((void *)(intptr_t) operand);
        if (flg != OK)
            HALT(0);
    }
    return 0;
}

void *READ(void *p) {
    int flg = 0;
    char *buf;
    buf = read_console();
    add_messange(buf);
    int_least16_t value;
    flg = decod_val(buf, &value);
    free(buf);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    flg = sc_memorySet((int)(intptr_t) p, value);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    InctructionCounter++;
    return (void *)(intptr_t) 0;
}

void *WRITE(void *p) {
    int flg = 0;
    int_least16_t value;
    flg = sc_memoryGet((int)(intptr_t) p, &value);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    char buf[6];
    print_memory_cell(buf, value);
    add_messange(buf);
    InctructionCounter++;
    return (void *)(intptr_t) 0;
}

void *HALT(void *p) {
    InctructionCounter = 0;
    sc_regSet(FLAG_IGNORE_IMP, 1);
    return (void *)(intptr_t) 0;
}

void *LOAD(void *p) {
    int flg = 0;
    int_least16_t value;
    flg = sc_memoryGet((int)(intptr_t) p, &value);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    Accumulator = value;
    InctructionCounter++;
    return (void *)(intptr_t) 0;
}

void *STORE(void *p) {
    int flg = 0;
    flg = sc_memorySet((int)(intptr_t) p, Accumulator);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    InctructionCounter++;
    return (void *)(intptr_t) 0;
}

void *ADD(void *p) {
    int flg = 0;
    int_least16_t cell;
    flg = sc_memoryGet((int)(intptr_t) p, &cell);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    int_least16_t value;
    flg = sc_valueDecode(&value, cell);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    int_least16_t accum;
    flg = sc_valueDecode(&accum, Accumulator);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    value &= 0xffffbfff;
    accum += value;
    sc_valueEncode(accum, &Accumulator);
    InctructionCounter++;
    return (void *)(intptr_t) 0;
}

void *SUB(void *p) {
    int flg = 0;
    int_least16_t cell;
    flg = sc_memoryGet((int)(intptr_t) p, &cell);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    int_least16_t value;
    flg = sc_valueDecode(&value, cell);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    int_least16_t accum;
    flg = sc_valueDecode(&accum, Accumulator);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    value &= 0xffffbfff;
    accum -= value;
    sc_valueEncode(accum, &Accumulator);
    InctructionCounter++;
    return (void *)(intptr_t) 0;
}

void *DIVIDE(void *p) {
    int flg = 0;
    int_least16_t cell;
    flg = sc_memoryGet((int)(intptr_t) p, &cell);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    int_least16_t value;
    flg = sc_valueDecode(&value, cell);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    int_least16_t accum;
    flg = sc_valueDecode(&accum, Accumulator);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    value &= 0xffffbfff;
    if (value == 0) {
        sc_regSet(FLAG_DIVISION_BY_ZERO, 1);
        return (void *)(intptr_t) -1;
    }
    accum /= value;
    sc_valueEncode(accum, &Accumulator);
    InctructionCounter++;
    return (void *)(intptr_t) 0;
}

void *MUL(void *p) {
    int flg = 0;
    int_least16_t cell;
    flg = sc_memoryGet((int)(intptr_t) p, &cell);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    int_least16_t value;
    flg = sc_valueDecode(&value, cell);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    int_least16_t accum;
    flg = sc_valueDecode(&accum, Accumulator);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    value &= 0xffffbfff;
    accum *= value;
    sc_valueEncode(accum, &Accumulator);
    InctructionCounter++;
    return (void *)(intptr_t) 0;
}

void *JUMP(void *p) {
    if (((uint_least16_t)(intptr_t) p) > 99 || ((uint_least16_t)(intptr_t) p) < 0) {
        sc_regSet(FLAG_SEGMENTATION_FAULT, 1);
        return (void *)(intptr_t) -1;
    }
    char number = (char)(intptr_t) p;
    InctructionCounter = (uint_least16_t) number;
    return (void *)(intptr_t) 0;
}

void *JNEG(void *p) {
    if (((uint_least16_t)(intptr_t) p) > 99 || ((uint_least16_t)(intptr_t) p) < 0) {
        sc_regSet(FLAG_SEGMENTATION_FAULT, 1);
        return (void *)(intptr_t) -1;
    }
    char number = (char)(intptr_t) p;
    int_least16_t accum;
    int flg = sc_valueDecode(&accum, Accumulator);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    if (accum < 0) {
        InctructionCounter = (uint_least16_t) number;
    } else
        InctructionCounter++;
    return (void *)(intptr_t) 0;
}

void *JZ(void *p) {
    if (((uint_least16_t)(intptr_t) p) > 99 || ((uint_least16_t)(intptr_t) p) < 0) {
        sc_regSet(FLAG_SEGMENTATION_FAULT, 1);
        return (void *)(intptr_t) -1;
    }
    char number = (char)(intptr_t) p;
    int_least16_t accum;
    int flg = sc_valueDecode(&accum, Accumulator);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    accum &= 0xffffbfff;
    if (accum == 0) {
        InctructionCounter = (uint_least16_t) number;
    } else
        InctructionCounter++;
    return (void *)(intptr_t) 0;
}

void *NOT(void *p) {
    int_least16_t accum;
    int flg = sc_valueDecode(&accum, Accumulator);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    accum &= 0xffffbfff;
    accum = ~accum;
    sc_valueEncode(accum, &Accumulator);
    int_least16_t cell;
    sc_valueEncode(accum, &cell);
    sc_memorySet((int)(intptr_t) p, cell);
    InctructionCounter++;
    return (void *)(intptr_t) 0;
}

void *AND(void *p) {
    int_least16_t accum;
    int flg = sc_valueDecode(&accum, Accumulator);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    accum &= 0xffffbfff;
    int_least16_t value;
    flg = sc_memoryGet((int)(intptr_t) p, &value);
    if (flg != OK) {
        return (void *)(intptr_t) -1;
    }
    value &= 0xffffbfff;
    int_least16_t res;
    res = value & accum;
    sc_valueEncode(res, &Accumulator);
    InctructionCounter++;
    return (void *)(intptr_t) 0;
}