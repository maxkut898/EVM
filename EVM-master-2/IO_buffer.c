//
// Created by evgenii on 12.04.17.
//

#include <string.h>
#include "IO_buffer.h"

int add_messange(char *messange) {
    for (uint i = 0; i < 5; i++) {
        if(IO[i] == NULL) {
            IO[i] = strdup(messange);
            number_of_string = i + 1;
            return 0;
        }
    }
    if (IO[0] != NULL)
        free(IO[0]);
    for (int i = 1; i < 6; ++i) {
        IO[i - 1] = IO[i];
    }
    free(IO[4]);
    IO[4] = strdup(messange);
    number_of_string = 5;
    draw_IO();
    return 0;
}

uint get_num_string_IO() {
    return number_of_string;
}

char *read_console() {
    char *buffer;
    buffer = calloc(100, sizeof(char));
    rk_mytermregime(1, 0, 0, 1, 1);
    ssize_t  num = read(STDIN_FILENO, buffer, 100);
    buffer[num - 1] = '\0';
    rk_mytermregime(0, 0, 1, 0, 1);
    return buffer;
}
