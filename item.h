#ifndef ITEM_H
#define ITEM_H

#include "types.h"

void print_item(const struct item_definition *item);
void add_item(enum item item_id, struct character *character);
void print_item_instance(const struct character *character);
extern struct item_definition item_table[SIZE_ITEM];

#endif
