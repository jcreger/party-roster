#include "character.h"
#include "render.h"
#include "types.h"
#include "item.h"

#include <stdbool.h>
#include <stdio.h>

int main(void) {
    character party[MAX_PARTY];
    size_t party_size = 0;
    static bool running = true;

    while (running) {
        menu_option user_input = render_menu(party_size);
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
        case MENU_INVENTORY:
            open_inventory(party, party_size);
            break;
        case MENU_ITEM_ADD:
            add_inventory(party, party_size);
            break;
        case MENU_QUIT:
            running = false;
            clear_terminal();
            return 0;
        default:
            return 1;
        }
    }
}
