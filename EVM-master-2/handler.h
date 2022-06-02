//
// Created by evgenii on 12.04.17.
//

#ifndef COMPUTER_HANDLER_H
#define COMPUTER_HANDLER_H

#include "MyReadKey.h"
#include "draw_interface.h"
#include <signal.h>
#include "signal.h"
#include "cpu.h"

void handler_run();
void handler_step();
void handler_reset();
void handler_II();
void handler_loud();
void handler_save();
void handler_loud_cell_memory(uint place_x, uint place_y);
void handler_loud_accumulation();
void handler_loud_instr_coutner();
void handler_key(keys key);
int main_computer();

int decod_val_com(char *buf, int_least16_t *cell);
int decod_val(char *buf, int_least16_t *cell);


#endif //COMPUTER_HANDLER_H
