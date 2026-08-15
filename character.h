#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"
#include <stddef.h>

extern struct stats stats_table[JOB_COUNT];

void add_character(struct character party[], size_t *party_size);
void remove_character(struct character party[], size_t *party_size);
void view_character(const struct character party[], size_t party_size);
void change_character(struct character party[], const size_t party_size);
void sort_character(struct character party[], size_t party_size);

#endif
