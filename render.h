#ifndef RENDER_H
#define RENDER_H

#include "character.h"
#include <stdlib.h>

enum menu_option {
    MENU_ADD,
    MENU_VIEW,
    MENU_CHANGE,
    MENU_REMOVE,
    MENU_SORT,
    MENU_COUNT
};

enum sort {
    SORT_NAME,
    SORT_STRENGTH,
    SORT_AGILITY,
    SORT_INTELLIGENCE,
    SORT_STAMINA,
    SORT_RESILIENCE,
    SORT_SPIRIT,
    SORT_COUNT
};

enum order { ORDER_ASCENDING, ORDER_DESCENDING, ORDER_COUNT };

const char *get_job_string(const enum job job_id);
const char *get_menu_string(const enum menu_option option_id);
const char *get_sort_string(const enum sort sort_id);
const char *get_order_string(const enum order order_id);
void render_party(const struct character party[], size_t party_size);
void render_quit(const size_t count);
enum menu_option render_menu(const size_t party_size);
void render_party_character(const struct character character);
void render_sort();
void render_order();

#endif
