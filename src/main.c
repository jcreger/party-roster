#include "character.h"
#include "input.h"
#include "item.h"
#include "print.h"
#include "types.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// print the main menu and return valid user input
menu_main_e menu_main(const size_t party_size) {
    int input;
    status_e input_status;

    while (true) {
        clear_terminal();
        print_menu(party_size);

        input_status = clean_input_int(&input);
        switch (validate_input(&input, &input_status, MENU_COUNT - 1)) {
        case INPUT_VALID:
            return input;
        case INPUT_Q:
            return MENU_QUIT;
        case INPUT_ERROR:
            clear_terminal();
            print_status(input_status);
            wait_enter();
            break;
        }
    }
}

void free_party(character_s party[], const size_t party_size) {
    size_t memory = 0;

    for (size_t i = 0; i < party_size; i++) {
        memory = memory + sizeof(item_instance_s) * party[i].inventory_size;
        free(party[i].inventory);
        party[i].inventory = NULL;
        party[i].inventory_size = 0;
    }
    clear_terminal();
    printf("FREE: %zu BYTES OF MEMORY\n", memory);
    wait_enter();
}

int main(void) {
    character_s party[MAX_PARTY];
    size_t party_size = 0;
    menu_main_e input;

    while (true) {
        input = menu_main(party_size);
        switch (input) {
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
            view_inventory(party, party_size);
            break;
        case MENU_ITEM_ADD:
            add_inventory(party, party_size);
            break;
        case MENU_ITEM_REMOVE:
            remove_inventory(party, party_size);
            break;
        case MENU_QUIT:
            free_party(party, party_size);
            clear_terminal();
            return 0;
        default:
            return 1;
        }
    }
}
