#ifndef ITEM_H
#define ITEM_H

#include "types.h"

void print_item(const struct item_definition *item);
void print_item_instance(const struct character *character);
void add_item(enum item item_id, struct item_instance inventory[],
              size_t *inventory_size);
void remove_item(enum item item_id, struct item_instance inventory[],
                 size_t *inventory_size);
extern struct item_definition item_table[MAX_ITEM];

#endif
