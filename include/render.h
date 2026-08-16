#ifndef RENDER_H
#define RENDER_H

#include "types.h"
#include <stddef.h>

const char *get_job_string(const job job_id);
const char *get_menu_string(const menu_option option_id);
const char *get_sort_string(const sort sort_id);
const char *get_order_string(const order order_id);
const char *get_slot_string(const armor_slot slot_id);
const char *get_type_string(const item_type type_id);

void print_item(const item_definition item);
void print_inventory(const item_instance inventory[], const size_t inventory_size);

menu_option render_menu(const size_t party_size);

void render_party(const character party[], size_t party_size);
void render_back(void);
void render_party_character(const character character);
void render_sort();
void render_order();
void render_item_table(void);

void read_status(status status_id);

void clear_terminal();

#endif
