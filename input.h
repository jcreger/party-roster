#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

extern const int MENU_DELAY;

enum status {
    STATUS_OKAY,
    STATUS_NOT_FOUND,
    STATUS_INVALID_INPUT,
    STATUS_NULL_INPUT,
    STATUS_LONG_INPUT,
    STATUS_LIST_FULL,
    STATUS_EMPTY,
    STATUS_INVALID_OPTION
};

enum selection { SELECTION_VALID, SELECTION_QUIT, SELECTION_INVALID };

int clean_input(char string[], const size_t string_size);
enum status clean_input_int(int *num, int digit_count);
enum selection validate_input(int *input, const enum status input_status,
                              const size_t count);
void wait_enter();
void read_status(enum status status_id);
void clear_terminal();
void delay(int milliseconds);

#endif
