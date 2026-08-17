#ifndef print_H
#define print_H

#include "types.h"

#include <stddef.h>

void clear_terminal(void);

const char *get_job_string(const job_e job_id);
const char *get_menu_string(const menu_main_e option_id);
const char *get_sort_string(const sort_e sort_id);
const char *get_order_string(const order_e order_id);
const char *get_slot_string(const armor_slot_e slot_id);
const char *get_type_string(const item_type_e type_id);

void print_status(const status_e status_id);
void print_party(const character_s party[], const size_t party_size);
void print_back(void);
void print_menu(const size_t party_size);
void print_character(const character_s character);
void print_sort(void);
void print_order(void);
void print_item(const item_definition_s item);
void print_inventory(const item_instance_s inventory[],
                     const size_t inventory_size);
void print_item_table(void);
void print_carrot(void);

#endif
