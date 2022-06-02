//
// Created by evgenii on 31.03.17.
//

#ifndef COMPUTER_MYREADKEY_H
#define COMPUTER_MYREADKEY_H

#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

/*
    tcflag_t c_iflag;      // режимы ввода
    tcflag_t c_oflag;      // режимы вывода
    tcflag_t c_cflag;      // режимы управления
    tcflag_t c_lflag;      // режимы локали
    cc_t c_cc[NCCS];       // управляющие символы
c_iflag - флаги констант:
*/
typedef enum {
    KEY_l,
    KEY_s,
    KEY_r,
    KEY_t,
    KEY_i,
    KEY_p,
    KEY_d,
    KEY_q,
    KEY_f5,
    KEY_f6,
    KEY_up,
    KEY_down,
    KEY_left,
    KEY_right,
    KEY_enter,
    KEY_other
} keys;

int rk_readkey(keys *key);
int rk_mytermsave(void);
int rk_mytermrestore(void);
int rk_mytermregime(int regime, cc_t vtime, cc_t vmin, int echo, int sigint);


#endif //COMPUTER_MYREADKEY_H
