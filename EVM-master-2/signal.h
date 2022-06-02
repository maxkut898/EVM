//
// Created by evgenii on 14.04.17.
//

#ifndef COMPUTER_SIGNAL_H
#define COMPUTER_SIGNAL_H

#include <sys/time.h>
#include <signal.h>
#include <zconf.h>
#include "MySimpleComputer.h"
#include "draw_interface.h"
#include "cpu.h"

void set_signals();
void timer_handler(int sig);
void usrsig_handler(int sig);
void start_timer(void);

#endif //COMPUTER_SIGNAL_H
