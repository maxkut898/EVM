//
// Created by evgenii on 21.04.17.
//

#ifndef COMPUTER_ALU_H
#define COMPUTER_ALU_H

#include "MySimpleComputer.h"
#include "draw_interface.h"
#include "handler.h"

typedef void *(* handler_command)(void *p);

handler_command table_command[100];

bool for_alu[100];

void table_command_init();

int alu(int command, int operand);
int cu();

void *READ(void *p);
void *WRITE(void *p);

void *LOAD(void *p);
void *STORE(void *p);

void *ADD(void *p);
void *SUB(void *p);
void *DIVIDE(void *p);
void *MUL(void *p);

void *JUMP(void *p);
void *JNEG(void *p);
void *JZ(void *p);
void *HALT(void *p);

void *NOT(void *p);
void *AND(void *p);
#endif //COMPUTER_ALU_H
