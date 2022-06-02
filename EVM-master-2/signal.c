//
// Created by evgenii on 14.04.17.
//

#include "signal.h"


void set_signals() {
    signal(SIGALRM, timer_handler);
    signal(SIGUSR1, usrsig_handler);
}


void timer_handler(int sig) {
    int val;
    sc_regGet(FLAG_IGNORE_IMP, &val);
    if (val) {
        return;
    }
    cu();
    draw_interface();
}


void usrsig_handler(int sig)
{
    sc_memoryInit();
    sc_regInit();
    sc_AccumIniit();
    sc_InstractionCounterInit();
    sc_regSet(FLAG_IGNORE_IMP, 1);
    draw_interface();
}

void start_timer(void) {
    struct itimerval nval, oval;

    nval.it_interval.tv_sec = 0;
    nval.it_interval.tv_usec = 80000;
    nval.it_value.tv_sec = 0;
    nval.it_value.tv_usec = 40000;

    setitimer(ITIMER_REAL, &nval, &oval);
}
