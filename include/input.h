#ifndef INPUT_H
#define INPUT_H

#include "types.h"
#include <stddef.h>

extern const int MENU_DELAY;

void delay(int milliseconds);
status_e clean_input_string(char string[], const size_t string_size);
status_e clean_input_int(int *num);
input_e validate_input(int *input, status_e *input_status, const size_t count);
void wait_enter(void);

#endif
