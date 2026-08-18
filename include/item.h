#ifndef ITEM_H
#define ITEM_H

#include "types.h"

#include <stddef.h>

void add_item(const item_e item, item_instance_s **inventory,
              size_t *inventory_size);
item_e select_item(void);
void remove_item(const item_e item, item_instance_s **inventory,
                 size_t *inventory_size);
void view_inventory(const character_s party[], const size_t party_size);
void add_inventory(character_s party[], const size_t party_size);
item_e select_inventory(const item_instance_s inventory[],
                        const size_t inventory_size);
void remove_inventory(character_s party[], const size_t party_size);

#endif
