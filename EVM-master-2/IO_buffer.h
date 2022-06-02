//
// Created by evgenii on 12.04.17.
//

#ifndef COMPUTER_IO_BUFFER_H
#define COMPUTER_IO_BUFFER_H

#include <stdlib.h>
#include <string.h>
#include "MyReadKey.h"
#include "draw_interface.h"

char *IO[6];
uint number_of_string;

int add_messange(char *messange);
char *read_console();

uint get_num_string_IO();

#endif //COMPUTER_IO_BUFFER_H
