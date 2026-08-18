#include "input.h"
#include "print.h"
#include "types.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include <strings.h>
#endif

int string_case_compare(char str1[], char str2[]) {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    return strcasecmp(str1, str2);
#endif
#if defined(_WIN32) || defined(_WIN64)
    return _stricmp(str1, str2);
#endif
}

void free_character(character_s *character) {
    clear_terminal();
    printf("FREE: %zu BYTES OF MEMORY\n",
           sizeof(item_instance_s) * character->inventory_size);
    wait_enter();
    free(character->inventory);
    character->inventory = NULL;
    character->inventory_size = 0;
}

// swap function for bubble sort
static void struct_swap(character_s *a, character_s *b) {
    character_s temp = *a;
    *a = *b;
    *b = temp;
}

// bubble sort party by type and order
static void party_sort(character_s party[], size_t party_size, sort_e sort_id,
                       order_e order_id) {
    if (party_size == 0) {
        return;
    }

    for (size_t i = 0; i < party_size - 1; i++) {
        bool swapped = false;
        for (size_t j = 0; j < party_size - i - 1; j++) {
            uint8_t a, b;
            char str1[LEN_NAME_CHARACTER], str2[LEN_NAME_CHARACTER];
            switch (sort_id) {
            case SORT_STRENGTH:
                a = party[j].stats.strength;
                b = party[j + 1].stats.strength;
                break;
            case SORT_AGILITY:
                a = party[j].stats.agility;
                b = party[j + 1].stats.agility;
                break;
            case SORT_INTELLIGENCE:
                a = party[j].stats.intelligence;
                b = party[j + 1].stats.intelligence;
                break;
            case SORT_STAMINA:
                a = party[j].stats.stamina;
                b = party[j + 1].stats.stamina;
                break;
            case SORT_RESILIENCE:
                a = party[j].stats.resilience;
                b = party[j + 1].stats.resilience;
                break;
            case SORT_SPIRIT:
                a = party[j].stats.spirit;
                b = party[j + 1].stats.spirit;
                break;
            case SORT_NAME:
                strcpy(str1, party[j].name);
                strcpy(str2, party[j + 1].name);
                break;
            default:
                clear_terminal();
                printf("ERROR: SORT UNDEFINED\n");
                wait_enter();
                return;
            }
            if (sort_id != SORT_NAME) {
                switch (order_id) {
                case ORDER_ASCENDING:
                    if (a > b) {
                        struct_swap(&party[j], &party[j + 1]);
                        swapped = true;
                    }
                    break;
                case ORDER_DESCENDING:
                    if (a < b) {
                        struct_swap(&party[j], &party[j + 1]);
                        swapped = true;
                    }
                    break;
                default:
                    clear_terminal();
                    printf("ERROR: ORDER UNDEFINED\n");
                    wait_enter();
                    return;
                }
            } else {
                switch (order_id) {
                case ORDER_ASCENDING:
                    if (string_case_compare(str1, str2) > 0) {
                        struct_swap(&party[j], &party[j + 1]);
                        swapped = true;
                    }
                    break;
                case ORDER_DESCENDING:
                    if (string_case_compare(str1, str2) < 0) {
                        struct_swap(&party[j], &party[j + 1]);
                        swapped = true;
                    }
                    break;
                default:
                    clear_terminal();
                    printf("ERROR: ORDER UNDEFINED\n");
                    wait_enter();
                    break;
                }
            }
        }
        if (!swapped) {
            return;
        }
    }
}

// assign name to character
static void add_name(character_s *character) {
    char name[LEN_NAME_CHARACTER];
    status_e name_status;

    while (true) {
        clear_terminal();
        printf("Name: ");
        name_status = clean_input_string(name, sizeof(name));
        if (name_status == STATUS_OK) {
            strcpy(character->name, name);
            return;
        } else {
            clear_terminal();
            print_status(name_status);
            wait_enter();
        }
    }
}

// assign job to character
static void add_job(character_s *character) {
    status_e input_status;
    int input;

    while (true) {
        clear_terminal();
        for (int i = 0; i < JOB_COUNT; i++) {
            printf("%d | %s\n", i + 1, get_job_string(i));
        }
        print_carrot();

        input_status = clean_input_int(&input);
        switch (validate_input(&input, &input_status, JOB_COUNT)) {
        case INPUT_VALID:
            character->job = input;
            character->stats = stats_table[input];
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

// add new character to party
void add_character(character_s party[], size_t *party_size) {
    int index = *party_size;

    if (*party_size >= MAX_PARTY) {
        clear_terminal();
        print_status(STATUS_PARTY_FULL);
        wait_enter();
        return;
    }

    add_name(&party[index]);
    add_job(&party[index]);

    party[index].inventory_size = 0;
    party[index].inventory = NULL;

    (*party_size)++;
    clear_terminal();
    print_character(party[index]);
    wait_enter();
}

// remove character from party and destroy memory
void remove_character(character_s party[], size_t *party_size) {
    status_e input_status;
    int input;

    if (*party_size <= 0) {
        clear_terminal();
        print_status(STATUS_PARTY_EMPTY);
        wait_enter();
        return;
    }

    while (true) {
        clear_terminal();
        print_party(party, *party_size);
        print_back();
        print_carrot();

        input_status = clean_input_int(&input);
        switch (validate_input(&input, &input_status, *party_size)) {
        case INPUT_VALID:
            free_character(&party[input]);
            for (size_t i = input; i < *party_size - 1; i++) {
                party[i] = party[i + 1];
            }
            memset(&party[*party_size - 1], '\0', sizeof(character_s));
            (*party_size)--;
            if (*party_size == 0) {
                return;
            }
            break;
        case INPUT_Q:
            return;
        case INPUT_ERROR:
            clear_terminal();
            print_status(input_status);
            wait_enter();
            break;
        }
    }
}

// view party character information
void view_character(const character_s party[], const size_t party_size) {
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
        print_back();
        print_carrot();

        input_status = clean_input_int(&input);
        switch (validate_input(&input, &input_status, party_size)) {
        case INPUT_VALID:
            clear_terminal();
            print_character(party[input]);
            wait_enter();
            break;
        case INPUT_Q:
            return;
        case INPUT_ERROR:
            clear_terminal();
            print_status(input_status);
            wait_enter();
            break;
        }
    }
}

// change job of character
void change_character(character_s party[], const size_t party_size) {
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
        print_back();
        print_carrot();

        input_status = clean_input_int(&input);
        switch (validate_input(&input, &input_status, party_size)) {
        case INPUT_VALID:
            add_job(&party[input]);
            clear_terminal();
            print_character(party[input]);
            wait_enter();
            break;
        case INPUT_Q:
            return;
        case INPUT_ERROR:
            clear_terminal();
            print_status(input_status);
            wait_enter();
            break;
        }
    }
}

// sort party character by sort type and order
void sort_character(character_s party[], const size_t party_size) {
    status_e sort_status, order_status;
    int sort, order;

    if (party_size <= 0) {
        clear_terminal();
        print_status(STATUS_PARTY_EMPTY);
        wait_enter();
        return;
    }

    while (true) {
        clear_terminal();
        print_sort();
        print_back();
        print_carrot();

        sort_status = clean_input_int(&sort);
        switch (validate_input(&sort, &sort_status, SORT_COUNT)) {
        case INPUT_VALID:
            while (true) {
                clear_terminal();
                print_order();
                print_carrot();

                order_status = clean_input_int(&order);
                switch (validate_input(&order, &order_status, ORDER_COUNT)) {
                case INPUT_VALID:
                    party_sort(party, party_size, sort, order);
                    clear_terminal();
                    print_party(party, party_size);
                    wait_enter();
                    return;
                case INPUT_Q:
                    break;
                case INPUT_ERROR:
                    clear_terminal();
                    print_status(order_status);
                    wait_enter();
                    break;
                }
            }
            break;
        case INPUT_Q:
            return;
        case INPUT_ERROR:
            clear_terminal();
            print_status(sort_status);
            wait_enter();
            break;
        }
    }
}
