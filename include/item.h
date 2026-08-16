#ifndef ITEM_H
#define ITEM_H

#include "types.h"

#include <stddef.h>

void add_item(item_id item_id, item_instance inventory[],
              size_t *inventory_size);
void remove_item(item_id item_id, item_instance inventory[],
                 size_t *inventory_size);
void open_inventory(const character party[], const size_t party_size);
void add_inventory (character party[], size_t party_size);

#endif
