#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"
#include <stddef.h>

void add_character(character party[], size_t *party_size);
void remove_character(character party[], size_t *party_size);
void view_character(const character party[], size_t party_size);
void change_character(character party[], const size_t party_size);
void sort_character(character party[], size_t party_size);

#endif
