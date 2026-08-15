#include "input.h"
#include "render.h"
#include "types.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include <strings.h>
#endif

struct stats stats_table[JOB_COUNT] = {

    [JOB_FIGHTER] = {.strength = 20,
                     .agility = 10,
                     .intelligence = 5,
                     .stamina = 15,
                     .resilience = 10,
                     .spirit = 5},

    [JOB_THIEF] = {.strength = 10,
                   .agility = 20,
                   .intelligence = 5,
                   .stamina = 10,
                   .resilience = 15,
                   .spirit = 5},

    [JOB_MAGE] = {.strength = 5,
                  .agility = 5,
                  .intelligence = 20,
                  .stamina = 10,
                  .resilience = 10,
                  .spirit = 15},

    [JOB_ROGUE] = {.strength = 15,
                   .agility = 20,
                   .intelligence = 5,
                   .stamina = 10,
                   .resilience = 10,
                   .spirit = 5},

    [JOB_PALADIN] = {.strength = 15,
                     .agility = 5,
                     .intelligence = 10,
                     .stamina = 15,
                     .resilience = 10,
                     .spirit = 10},

    [JOB_ASSASSIN] = {.strength = 10,
                      .agility = 15,
                      .intelligence = 15,
                      .stamina = 10,
                      .resilience = 10,
                      .spirit = 5},
};

int string_case_compare(char str1[], char str2[]) {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    return strcasecmp(str1, str2);
#endif
#if defined(_WIN32) || defined(_WIN64)
    return _stricmp(str1, str2);
#endif
}

// Helper swap function for bubble sort
void struct_swap(struct character *a, struct character *b) {
    struct character temp = *a;
    *a = *b;
    *b = temp;
}

// Bubble sort that handles type and order
void array_sort(struct character party[], size_t array_size, enum sort sort_id,
                enum order order_id) {
    for (size_t i = 0; i < array_size; i++) {
        bool swapped = false;
        for (size_t j = 0; j < array_size - i - 1; j++) {
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
            case SORT_COUNT:
                clear_terminal();
                printf("%s", get_sort_string(SORT_COUNT));
                break;
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
                case ORDER_COUNT:
                    clear_terminal();
                    printf("%s", get_order_string(ORDER_COUNT));
                    break;
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
                case ORDER_COUNT:
                    clear_terminal();
                    printf("%s", get_order_string(ORDER_COUNT));
                    break;
                }
            }
        }
        if (!swapped) {
            return;
        }
    }
}

// Allows the user to enter and assign a name to a character
static void add_name(struct character *character) {
    char name[LEN_NAME_CHARACTER];
    enum status name_status;
    while (true) {
        clear_terminal();
        printf("Name: ");
        name_status = clean_input(name, sizeof(name));
        if (name_status == STATUS_OKAY) {
            strcpy(character->name, name);
            return;
        } else {
            read_status(name_status);
        }
    }
}

// Allows the user to select and assign a job to a character
static void add_job(struct character *character) {
    enum status job_status;
    int job_id;
    while (true) {
        clear_terminal();
        for (int i = 0; i < JOB_COUNT; i++) {
            printf("%d| %s\n", i + 1, get_job_string(i));
        }
        printf("\n> ");
        job_status = clean_input_int(&job_id, 1);
        if (job_status == STATUS_OKAY) {
            job_id--;
            if (job_id < JOB_COUNT && job_id >= 0) {
                character->job = job_id;
                character->stats = stats_table[job_id];
                return;
            } else {
                job_status = STATUS_INVALID_OPTION;
                read_status(job_status);
            }
        } else {
            read_status(job_status);
        }
    }
}

// Creates a new character struct in party
void add_character(struct character party[], size_t *party_size) {
    int index = *party_size;
    if (*party_size < SIZE_PARTY) {
        add_name(&party[index]);
        add_job(&party[index]);
        party[index].inventory_size = 0;
        (*party_size)++;
        render_party_character(party[index]);
    } else {
        read_status(STATUS_LIST_FULL);
    }
}

// Removes a character from the party array by shifting values to the left then
// setting the memory to 0
void remove_character(struct character party[], size_t *party_size) {
    enum status input_status;
    int input;
    if (*party_size > 0) {
        while (true) {
            render_party(party, *party_size);
            render_back(*party_size);
            input_status = clean_input_int(&input, 1);
            switch (validate_input(&input, input_status, *party_size, true)) {
            case SELECTION_VALID:
                for (size_t i = input; i < *party_size - 1; i++) {
                    party[i] = party[i + 1];
                }
                memset(&party[*party_size - 1], '\0', sizeof(struct character));
                (*party_size)--;
                if (*party_size == 0) {
                    return;
                }
                break;
            case SELECTION_BACK:
                return;
            case SELECTION_INVALID:
                break;
            }
        }
    } else {
        read_status(STATUS_EMPTY);
    }
}

// Opens menu to select invididual characters and view their name, job, stats
void view_character(const struct character party[], size_t party_size) {
    int input;
    enum status input_status;
    if (party_size > 0) {
        while (true) {
            render_party(party, party_size);
            render_back(party_size);
            input_status = clean_input_int(&input, 1);
            switch (validate_input(&input, input_status, party_size, true)) {
            case SELECTION_VALID:
                render_party_character(party[input]);
                break;
            case SELECTION_BACK:
                return;
            case SELECTION_INVALID:
                break;
            }
        }
    } else {
        read_status(STATUS_EMPTY);
    }
}

// Allows the user to swap the job of a party member
void change_character(struct character party[], const size_t party_size) {
    int input;
    enum status input_status;
    if (party_size > 0) {
        while (true) {
            render_party(party, party_size);
            render_back(party_size);
            input_status = clean_input_int(&input, 1);
            switch (validate_input(&input, input_status, party_size, true)) {
            case SELECTION_VALID:
                add_job(&party[input]);
                render_party_character(party[input]);
                break;
            case SELECTION_BACK:
                return;
            case SELECTION_INVALID:
                break;
            }
        }
    } else {
        read_status(STATUS_EMPTY);
    }
}

// Opens a menu that allows the user to select a sort method then order
void sort_character(struct character party[], size_t party_size) {
    enum status sort_status, order_status;
    int sort, order;
    if (party_size > 0) {
        while (true) {
            render_sort();
            render_back(SORT_COUNT);
            sort_status = clean_input_int(&sort, 1);
            switch (validate_input(&sort, sort_status, SORT_COUNT, true)) {
            case SELECTION_VALID:
                while (true) {
                    render_order();
                    printf("\n> ");
                    order_status = clean_input_int(&order, 1);
                    switch (validate_input(&order, order_status, ORDER_COUNT, true)) {
                    case SELECTION_VALID:
                        array_sort(party, party_size, sort, order);
                        render_party(party, party_size);
                        wait_enter();
                        return;
                    case SELECTION_BACK:
                        read_status(STATUS_INVALID_OPTION);
                        break;
                    case SELECTION_INVALID:
                        break;
                    }
                }
                break;
            case SELECTION_BACK:
                return;
            case SELECTION_INVALID:
                break;
            }
        }
    } else {
        read_status(STATUS_EMPTY);
    }
}
