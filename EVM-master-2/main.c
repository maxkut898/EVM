//
// Created by jonn on 17.02.17.
//

#include "draw_interface.h"
#include "handler.h"

int main() {
    sc_memoryInit();
    sc_regInit();
    sc_AccumIniit();
    sc_InstractionCounterInit();
    set_signals();
    table_command_init();
    sc_regSet(FLAG_IGNORE_IMP, 1);
    start_timer();
    main_computer();
    return 0;
}
