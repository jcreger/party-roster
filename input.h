#ifndef INPUT_H
#define INPUT_H

#include "types.h"
#include <stdio.h>

extern const int MENU_DELAY;

int clean_input(char string[], const size_t string_size);
enum status clean_input_int(int *num, int digit_count);
enum selection validate_input(int *input, const enum status input_status,
                              const size_t count);
void wait_enter();
void delay(int milliseconds);

#endif
