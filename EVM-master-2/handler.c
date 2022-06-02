//
// Created by evgenii on 12.04.17.
//

#include "handler.h"


void handler_key(keys key)
{
    switch (key) {
        case KEY_l:
            handler_loud();
            break;
        case KEY_s:
            handler_save();
            break;
        case KEY_r:
            handler_run();
            break;
        case KEY_t:
            handler_step();
            break;
        case KEY_i:
            handler_reset();
            break;
        case KEY_q:
            break;
        case KEY_up:
            place_cell_memory.x -= 1;
            if (place_cell_memory.x == -1) {
                place_cell_memory.x = 9;
            }
            draw_memory(place_cell_memory.x, place_cell_memory.y);
            draw_bigchar(place_cell_memory.x * 10 + place_cell_memory.y);
            break;
        case KEY_down:
            place_cell_memory.x += 1;
            if (place_cell_memory.x == 10) {
                place_cell_memory.x = 0;
            }
            draw_memory(place_cell_memory.x, place_cell_memory.y);
            draw_bigchar(place_cell_memory.x * 10 + place_cell_memory.y);
            break;
        case KEY_left:
            place_cell_memory.y -= 1;
            if (place_cell_memory.y == -1) {
                place_cell_memory.y = 9;
            }
            draw_memory(place_cell_memory.x, place_cell_memory.y);
            draw_bigchar(place_cell_memory.x * 10 + place_cell_memory.y);
            break;
        case KEY_right:
            place_cell_memory.y += 1;
            if (place_cell_memory.y == 10) {
                place_cell_memory.y = 0;
            }
            draw_memory(place_cell_memory.x, place_cell_memory.y);
            draw_bigchar(place_cell_memory.x * 10 + place_cell_memory.y);
            break;
        case KEY_f5:
            handler_loud_accumulation();
            break;
        case KEY_f6:
            handler_loud_instr_coutner();
            break;
        case KEY_p:
            handler_II();
            break;
        case KEY_d:break;
        case KEY_enter:
            handler_loud_cell_memory(place_cell_memory.x, place_cell_memory.y);
            break;
        case KEY_other:break;
    }

}

int main_computer()
{
    draw_interface();
    keys key;
    place_cell_memory.x = 0;
    place_cell_memory.y = 0;
    rk_mytermsave();
    do {
        rk_readkey(&key);
        handler_key(key);
    } while (key != KEY_q);
    rk_mytermrestore();
    uint16_t max_x, max_y;
    mt_getscreensize(&max_x, &max_y);
    mt_gotoXY(1, (uint16_t) (max_x - 1));
    return 0;
}

void handler_loud() {
    place_pointer.replace_pointer = 1;
    draw_load_save_memory();
    char *buf;
    buf = read_console();
    if (buf != NULL) {
        if (strcmp(buf, "\033") == 0) {
            free(buf);
            draw_interface();
            return;
        }
        int flag = sc_memoryLoad(buf);
        if (flag != OK) {
            add_messange("Error read memory file!");
        }
        free(buf);
    }
    draw_interface();
    place_pointer.replace_pointer = 0;
    return;
}

void handler_save() {
    place_pointer.replace_pointer = 1;
    draw_load_save_memory();
    char *buf;
    buf = read_console();
    if (buf != NULL) {
        if (strcmp(buf, "\033") == 0) {
            free(buf);
            draw_interface();
            return;
        }
        int flag = sc_memorySave(buf);
        if (flag != OK) {
            add_messange("Error save memory file!");
        }
        free(buf);
    }
    draw_interface();
    place_pointer.replace_pointer = 0;
    return;
}

void handler_loud_cell_memory(uint place_x, uint place_y) {
    place_pointer.replace_pointer = 1;
    draw_load_cell();
    char *buf;
    buf = read_console();
    if (buf != NULL) {
        if (strcmp(buf, "\033") == 0) {
            free(buf);
            draw_interface();
            return;
        }
        int_least16_t val;
        int flg = decod_val_com(buf, &val);
        if (flg == OK) {
            flg = sc_memorySet(place_x * 10 + place_y, val);
            if (flg != OK) {
                add_messange("Could not set cell value");
            }
        }
        else {
            add_messange("Could not set cell value");
        }
        free(buf);
    }
    draw_interface();
    place_pointer.replace_pointer = 0;
    return;
}

int decod_val_com(char *buf, int_least16_t *cell) {
    if (buf[0] == '+') {
        if (strlen(buf) > 5) {
            return -1;
        }
        int_least16_t val = (int_least16_t) atoi(&buf[1]);
        if ((val >= 0x1fff) || (val <= -0x1fff)) {
            return -1;
        }

        int flg = sc_valueEncode(val, cell);
        if (flg != OK) {
            return -1;
        }
    }
    else {
        if (strlen(buf) > 7) {
            return -1;
        }
        char *command_str = strtok(buf, ":");
        if (command_str == NULL) {
            return -1;
        }
        char *operand_str = strtok(NULL, ":");
        if (operand_str == NULL) {
            return -1;
        }
        int command = atoi(command_str);
        int operand = atoi(operand_str);
        if ((command < -127) || (command > 127) || (operand < -127) || (operand > 127)) {
            return -1;
        }
        int flg = sc_commandEncode((char) command, (char) operand, cell);
        if (flg != OK) {
            return -1;
        }
    }
    return 0;
}

void handler_loud_accumulation() {
    place_pointer.replace_pointer = 1;
    draw_load_cell();
    char *buf;
    buf = read_console();
    if (buf != NULL) {
        if (strcmp(buf, "\033") == 0) {
            free(buf);
            draw_interface();
            return;
        }
        int_least16_t val;
        int flg = decod_val(buf, &val);
        if (flg == OK) {
            Accumulator = val;
        }
        else {
            add_messange("Could not set cell value");
        }
        free(buf);
    }
    draw_interface();
    place_pointer.replace_pointer = 0;
    return;
}

int decod_val(char *buf, int_least16_t *cell) {
    if (buf[0] == '+') {
        if (strlen(buf) > 5) {
            return -1;
        }
        int_least16_t val = (int_least16_t) atoi(&buf[1]);
        if ((val >= 0x1fff) || (val <= -0x1fff)) {
            return -1;
        }
        int flg = sc_valueEncode(val, cell);
        if (flg != OK) {
            return -1;
        }
    } else {
        return -1;
    }
    return 0;
}

void handler_loud_instr_coutner() {
    place_pointer.replace_pointer = 1;
    draw_load_cell();
    char *buf;
    buf = read_console();
    if (buf != NULL) {
        if (strcmp(buf, "\033") == 0) {
            free(buf);
            draw_interface();
            return;
        }
        int_least16_t val;
        int flg = decod_val(buf, &val);
        if (flg == OK) {
            InctructionCounter = (uint_least8_t) val;
        }
        else {
            add_messange("Could not set cell value");
        }
        free(buf);
    }
    draw_interface();
    place_pointer.replace_pointer = 0;
    return;
}

void handler_reset() {
    raise(SIGUSR1);
}

void handler_run() {
    sc_regSet(FLAG_IGNORE_IMP, 0);
    draw_flag();
}

void handler_step() {
    sc_regSet(FLAG_IGNORE_IMP, 1);
    draw_flag();
    cu();
    draw_interface();
}

void handler_II() {
    int v;
    sc_regGet(FLAG_IGNORE_IMP, &v);
    if (v == 1)
        return;
    sc_regSet(FLAG_IGNORE_IMP, 1);
    draw_flag();
}
