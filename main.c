#include "character.h"
#include "input.h"
#include "item.h"
#include "render.h"
#include "types.h"
#include <stdbool.h>
#include <stdio.h>

int main(void) {
    struct character party[MAX_PARTY];
    size_t party_size = 0;
    static bool running = true;
    while (running) {
        enum menu_option user_input = render_menu(party_size);
        switch (user_input) {
        case MENU_ADD:
            add_character(party, &party_size);
            break;
        case MENU_VIEW:
            view_character(party, party_size);
            break;
        case MENU_CHANGE:
            change_character(party, party_size);
            break;
        case MENU_REMOVE:
            remove_character(party, &party_size);
            break;
        case MENU_SORT:
            sort_character(party, party_size);
            break;
        case MENU_QUIT:
            clear_terminal();
            if (party_size > 0) {
                struct item_instance *inv = party[0].inventory;
                size_t *inv_size = &party[0].inventory_size;
                add_item(ITEM_SWORD_COPPER, inv, inv_size);
                add_item(ITEM_SWORD_COPPER, inv, inv_size);
                add_item(ITEM_SWORD_COPPER, inv, inv_size);
                add_item(ITEM_CHEST_LEATHER, inv, inv_size);
                add_item(ITEM_POTION, inv, inv_size);
                add_item(ITEM_POTION, inv, inv_size);
                add_item(ITEM_KEY, inv, inv_size);
                add_item(ITEM_KEY, inv, inv_size);
                add_item(ITEM_KEY, inv, inv_size);
                add_item(ITEM_KEY, inv, inv_size);
                print_item_instance(&party[0]);
                wait_enter();
                clear_terminal();
            }
            running = false;
            clear_terminal();
            return 0;
        default:
            return 1;
        }
    }
}
