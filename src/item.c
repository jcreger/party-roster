#include "input.h"
#include "print.h"
#include "types.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void add_item(item_id item_id, item_instance inventory[],
              size_t *inventory_size) {
    if (*inventory_size >= MAX_INVENTORY) {
        read_status(STATUS_INV_FULL);
        return;
    }

    for (size_t i = 0; i < *inventory_size; i++) {
        if (inventory[i].item_id == item_id) {
            inventory[i].quantity++;
            return;
        }
    }

    inventory[*inventory_size].item_id = item_id;
    inventory[*inventory_size].quantity = 1;
    (*inventory_size)++;
    return;
}

item_id select_item(void) {
    int input;
    status input_status;

    while (true) {
        print_item_table();
        input_status = clean_input_int(&input, 1);
        switch (validate_input(&input, input_status, ITEM_COUNT)) {
        case SELECTION_VALID:
            return input;
        case SELECTION_BACK:
            break;
        case SELECTION_INVALID:
            break;
        }
    }
}

void remove_item(item_id item_id, item_instance inventory[],
                 size_t *inventory_size) {
    if (*inventory_size == 0) {
        read_status(STATUS_INV_EMPTY);
        return;
    }

    for (size_t i = 0; i < *inventory_size; i++) {
        if (inventory[i].item_id == item_id) {
            inventory[i].quantity--;
            if (inventory[i].quantity == 0) {
                for (size_t j = i; j < *inventory_size - 1; j++) {
                    inventory[j] = inventory[j + 1];
                }
                memset(&inventory[*inventory_size - 1], '\0',
                       sizeof(item_instance));
                (*inventory_size)--;
                i--;
            }
            return;
        }
    }

    return;
}

void view_inventory(const character party[], const size_t party_size) {
    int input;
    status input_status;

    if (party_size <= 0) {
        read_status(STATUS_EMPTY);
        return;
    }

    while (true) {
        print_party(party, party_size);
        input_status = clean_input_int(&input, 1);
        switch (validate_input(&input, input_status, party_size)) {
        case SELECTION_VALID:
            if (party[input].inventory_size <= 0) {
                read_status(STATUS_INV_EMPTY);
                return;
            }

            print_inventory(party[input].inventory,
                            party[input].inventory_size);
            wait_enter();
            return;
        case SELECTION_BACK:
            break;
        case SELECTION_INVALID:
            break;
            ;
        }
    }
}

void add_inventory(character party[], size_t party_size) {
    int input;
    status input_status;
    item_id item_id;

    if (party_size <= 0) {
        read_status(STATUS_EMPTY);
        return;
    }

    while (true) {
        print_party(party, party_size);
        input_status = clean_input_int(&input, 1);
        switch (validate_input(&input, input_status, party_size)) {
        case SELECTION_VALID:
            item_id = select_item();
            add_item(item_id, party[input].inventory,
                     &party[input].inventory_size);
            return;
        case SELECTION_BACK:
            break;
        case SELECTION_INVALID:
            break;
            ;
        }
    }
}

item_id select_remove_item(const item_instance inventory[],
                           const size_t inventory_size) {
    int input;
    status input_status;

    if (inventory_size <= 0) {
        read_status(STATUS_INV_EMPTY);
        return -1;
    }

    while (true) {
        print_inventory(inventory, inventory_size);
        input_status = clean_input_int(&input, 1);
        switch (validate_input(&input, input_status, inventory_size)) {
        case SELECTION_VALID:
            return inventory[input].item_id;
        case SELECTION_BACK:
            break;
        case SELECTION_INVALID:
            break;
        }
    }
}

void remove_inventory(character party[], size_t party_size) {
    int input;
    status input_status;
    item_id item_id;

    if (party_size <= 0) {
        read_status(STATUS_EMPTY);
        return;
    }

    while (true) {
        print_party(party, party_size);
        input_status = clean_input_int(&input, 1);
        switch (validate_input(&input, input_status, party_size)) {
        case SELECTION_VALID:
            if (party[input].inventory_size <= 0) {
                read_status(STATUS_INV_EMPTY);
                return;
            }

            item_id = select_remove_item(party[input].inventory,
                                         party[input].inventory_size);
            remove_item(item_id, party[input].inventory,
                        &party[input].inventory_size);
            return;
        case SELECTION_BACK:
            break;
        case SELECTION_INVALID:
            break;
            ;
        }
    }
}
