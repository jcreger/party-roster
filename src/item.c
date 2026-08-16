#include "input.h"
#include "render.h"
#include "types.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void add_item(item_id item_id, item_instance inventory[],
              size_t *inventory_size) {
    if (*inventory_size >= MAX_INVENTORY) {
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
        render_item_table();
        input_status = clean_input_int(&input, 1);
        switch (validate_input(&input, input_status, ITEM_COUNT)) {
        case SELECTION_VALID:
            return input;
        case SELECTION_BACK:
            read_status(STATUS_INVALID_OPTION);
            break;
        case SELECTION_INVALID:
            read_status(STATUS_INVALID_OPTION);
            break;
        }
    }
}

void remove_item(item_id item_id, item_instance inventory[],
                 size_t *inventory_size) {
    if (*inventory_size == 0) {
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

void open_inventory(const character party[], const size_t party_size) {
    int input;
    status input_status;

    if (party_size <= 0) {
        read_status(STATUS_EMPTY);
        return;
    }

    while (true) {
        render_party(party, party_size);
        input_status = clean_input_int(&input, 1);
        switch (validate_input(&input, input_status, party_size)) {
        case SELECTION_VALID:
            print_inventory(party[input].inventory,
                            party[input].inventory_size);
            return;
        case SELECTION_BACK:
            read_status(STATUS_INVALID_OPTION);
            break;
        case SELECTION_INVALID:
            read_status(STATUS_INVALID_OPTION);
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
        render_party(party, party_size);
        input_status = clean_input_int(&input, 1);
        switch (validate_input(&input, input_status, party_size)) {
        case SELECTION_VALID:
            item_id = select_item();
            add_item(item_id, party[input].inventory,
                     &party[input].inventory_size);
            return;
        case SELECTION_BACK:
            read_status(STATUS_INVALID_OPTION);
            break;
        case SELECTION_INVALID:
            read_status(STATUS_INVALID_OPTION);
            break;
            ;
        }
    }
}

/* void remove_inventory(character party[], size_t party_size) {
    int input;
    status input_status;
    item_id item_id;

    if (party_size <= 0) {
        return_status(STATUS_EMPTY);
    }

    while (true) {
    render_party(iparty
    }  */
