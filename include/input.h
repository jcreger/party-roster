#ifndef INPUT_H
#define INPUT_H

#include "types.h"
#include <stddef.h>

extern const int MENU_DELAY;

int clean_input(char string[], const size_t string_size);
status clean_input_int(int *num, int digit_count);
selection validate_input(int *input, const status input_status,
                         const size_t size);
void wait_enter();
void delay(int milliseconds);

#endif
