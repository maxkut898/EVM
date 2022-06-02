#ifndef __GUARD_SAT_H
#define __GUARD_SAT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MySimpleComputer.h"

#define ERR_ARG1 -1
#define ERR_ARG2 -2
#define ERR_ARG3 -3
#define ERR_ENCD -4
#define ERR_MANY -5
#define EMPTY_STR 1

char str2command(char *str);
int decod_val(char *buf, int_least16_t *cell);
int pars_line(char *str, int *addr, int_least16_t *value);
void print_error(char *line, int line_cnt, int err);
int test_argv_asm(char *argv[]);
int sat(int argc, char *argv[]);

#endif /* __GUARD_SAT_H */
