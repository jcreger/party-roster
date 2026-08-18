#include "input.h"
#include "print.h"
#include "types.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// add item into inventory instance
void add_item(const item_e item_id, item_instance_s **inventory,
              size_t *inventory_size) {
    item_instance_s *temp;

    if (*inventory_size >= MAX_INVENTORY) {
        clear_terminal();
        print_status(STATUS_INVENTORY_FULL);
        wait_enter();
        return;
    }

    for (size_t i = 0; i < *inventory_size; i++) {
        if ((*inventory)[i].item == item_id) {
            (*inventory)[i].quantity++;
            return;
        }
    }

    temp = realloc(*inventory, (*inventory_size + 1) * sizeof(item_instance_s));

    if (temp == NULL) {
        clear_terminal();
        print_status(STATUS_REALLOC_FAIL);
        wait_enter();
        return;
    }

    *inventory = temp;

    (*inventory)[*inventory_size].item = item_id;
    (*inventory)[*inventory_size].quantity = 1;
    (*inventory_size)++;
    return;
}

// return item id selected from item table
item_e select_item(void) {
    int input;
    status_e input_status;

    while (true) {
        clear_terminal();
        print_item_table();
        print_carrot();

        input_status = clean_input_int(&input);
        switch (validate_input(&input, &input_status, ITEM_COUNT)) {
        case INPUT_VALID:
            return input;
        case INPUT_Q:
            break;
        case INPUT_ERROR:
            clear_terminal();
            print_status(input_status);
            wait_enter();
            break;
        }
    }
}

// remove item from inventory instance
void remove_item(const item_e item, item_instance_s inventory[],
                 size_t *inventory_size) {
    if (*inventory_size <= 0) {
        clear_terminal();
        print_status(STATUS_INVENTORY_EMPTY);
        wait_enter();
        return;
    }

    for (size_t i = 0; i < *inventory_size; i++) {
        if (inventory[i].item == item) {
            inventory[i].quantity--;
            if (inventory[i].quantity == 0) {
                for (size_t j = i; j < *inventory_size - 1; j++) {
                    inventory[j] = inventory[j + 1];
                }
                memset(&inventory[*inventory_size - 1], '\0',
                       sizeof(item_instance_s));
                (*inventory_size)--;
                i--;
            }
            return;
        }
    }
    return;
}

// display inventory instance of party character
void view_inventory(const character_s party[], const size_t party_size) {
    int input;
    status_e input_status;

    if (party_size <= 0) {
        clear_terminal();
        print_status(STATUS_PARTY_EMPTY);
        wait_enter();
        return;
    }

    while (true) {
        clear_terminal();
        print_party(party, party_size);
        print_carrot();

        input_status = clean_input_int(&input);
        switch (validate_input(&input, &input_status, party_size)) {
        case INPUT_VALID:
            if (party[input].inventory_size <= 0) {
                clear_terminal();
                print_status(STATUS_INVENTORY_EMPTY);
                wait_enter();
                return;
            }

            clear_terminal();
            print_inventory(party[input].inventory,
                            party[input].inventory_size);
            wait_enter();
            return;
        case INPUT_Q:
            break;
        case INPUT_ERROR:
            clear_terminal();
            print_status(input_status);
            wait_enter();
            break;
            ;
        }
    }
}

// add item into inventory instance of party character
void add_inventory(character_s party[], const size_t party_size) {
    int input;
    status_e input_status;
    item_e item;

    if (party_size <= 0) {
        clear_terminal();
        print_status(STATUS_PARTY_EMPTY);
        wait_enter();
        return;
    }

    while (true) {
        clear_terminal();
        print_party(party, party_size);
        print_carrot();

        input_status = clean_input_int(&input);
        switch (validate_input(&input, &input_status, party_size)) {
        case INPUT_VALID:
            item = select_item();
            add_item(item, &party[input].inventory,
                     &party[input].inventory_size);
            return;
        case INPUT_Q:
            break;
        case INPUT_ERROR:
            clear_terminal();
            print_status(input_status);
            wait_enter();
            break;
        }
    }
}

// return item id selected from inventory
item_e select_inventory(const item_instance_s inventory[],
                        const size_t inventory_size) {
    int input;
    status_e input_status;

    if (inventory_size <= 0) {
        clear_terminal();
        print_status(STATUS_INVENTORY_EMPTY);
        wait_enter();
        return ITEM_INVALID;
    }

    while (true) {
        clear_terminal();
        print_inventory(inventory, inventory_size);
        print_carrot();

        input_status = clean_input_int(&input);
        switch (validate_input(&input, &input_status, inventory_size)) {
        case INPUT_VALID:
            return inventory[input].item;
        case INPUT_Q:
            break;
        case INPUT_ERROR:
            clear_terminal();
            print_status(input_status);
            wait_enter();
            break;
        }
    }
}

// remove item from inventory instance of party character
void remove_inventory(character_s party[], const size_t party_size) {
    int input;
    status_e input_status;
    item_e item;

    if (party_size <= 0) {
        clear_terminal();
        print_status(STATUS_PARTY_EMPTY);
        wait_enter();
        return;
    }

    while (true) {
        clear_terminal();
        print_party(party, party_size);
        print_carrot();

        input_status = clean_input_int(&input);
        switch (validate_input(&input, &input_status, party_size)) {
        case INPUT_VALID:
            if (party[input].inventory_size <= 0) {
                clear_terminal();
                print_status(STATUS_INVENTORY_EMPTY);
                wait_enter();
                return;
            }

            item = select_inventory(party[input].inventory,
                                    party[input].inventory_size);
            if (item == ITEM_INVALID) {
                return;
            }

            remove_item(item, party[input].inventory,
                        &party[input].inventory_size);
            return;
        case INPUT_Q:
            break;
        case INPUT_ERROR:
            clear_terminal();
            print_status(input_status);
            wait_enter();
            break;
            ;
        }
    }
}
