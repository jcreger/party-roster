#include "character.h"
#include "input.h"
#include "types.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// returns job string literal to be used in printf
const char *get_job_string(const enum job job_id) {
    switch (job_id) {
    case JOB_FIGHTER:
        return "Fighter";
    case JOB_THIEF:
        return "Thief";
    case JOB_MAGE:
        return "Mage";
    case JOB_ROGUE:
        return "Rogue";
    case JOB_PALADIN:
        return "Paladin";
    case JOB_ASSASSIN:
        return "Assassin";
    default:
        return "JOB UNDEFINED";
    }
}

// returns menu string literal to be used in printf
const char *get_menu_string(const enum menu_option option_id) {
    switch (option_id) {
    case MENU_ADD:
        return "Add a character";
    case MENU_VIEW:
        return "View party";
    case MENU_CHANGE:
        return "Change job";
    case MENU_REMOVE:
        return "Remove a character";
    case MENU_SORT:
        return "Sort characters";
    default:
        return "MENU UNDEFINED";
    }
}

// returns sort string literal to be used in printf
const char *get_sort_string(const enum sort sort_id) {
    switch (sort_id) {
    case SORT_NAME:
        return "Sort by Name";
    case SORT_STRENGTH:
        return "Sort by Strength";
    case SORT_AGILITY:
        return "Sort by Agility";
    case SORT_INTELLIGENCE:
        return "Sort by Intelligence";
    case SORT_STAMINA:
        return "Sort by Stamina";
    case SORT_RESILIENCE:
        return "Sort by Resilience";
    case SORT_SPIRIT:
        return "Sort by Spirit";
    default:
        return "SORT UNDEFINED";
    }
}

// returns order string literal to be used in printf
const char *get_order_string(const enum order order_id) {
    switch (order_id) {
    case ORDER_ASCENDING:
        return "Order by Ascending";
    case ORDER_DESCENDING:
        return "Order by Descending";
    default:
        return "ORDER UNDEFINED";
    }
}

const char *get_slot_string(const enum armor_slot slot_id) {
    switch (slot_id) {
    case SLOT_HEAD:
        return "HELMET";
    case SLOT_CHEST:
        return "CHEST";
    case SLOT_LEG:
        return "LEGS";
    case SLOT_GLOVE:
        return "GLOVES";
    case SLOT_BOOT:
        return "BOOTS";
    default:
        return "SLOT UNDEFINED";
    }
}

void clear_terminal() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

// Renders a menu for the entire party
void render_party(const struct character party[], size_t party_size) {
    clear_terminal();
    for (size_t i = 0; i < party_size; i++) {
        printf("%zu| %s\n", i + 1, party[i].name);
    }
}

// Renders the quit menu
void render_quit(const size_t count) {
    printf("\n%zu| Quit\n\n", count + 1);
    printf("> ");
}

// Render the main menu and return valid user input, returns the user input
enum menu_option render_menu(const size_t party_size) {
    int input;
    enum status input_status;
    while (true) {
        clear_terminal();
        printf("%zu/%d Characters\n\n", party_size, SIZE_PARTY);
        for (int i = 0; i < MENU_COUNT; i++) {
            printf("%d| %s\n", i + 1, get_menu_string(i));
        }
        render_quit(MENU_COUNT);
        input_status = clean_input_int(&input, 1);
        switch (validate_input(&input, input_status, MENU_COUNT)) {
        case SELECTION_VALID:
            return input;
        case SELECTION_QUIT:
            return MENU_COUNT;
        case SELECTION_INVALID:
            break;
        }
    }
}

// Renders information of a character
void render_party_character(const struct character character) {
    clear_terminal();
    printf("Name: %s\n\nJob: %s\n\nStrength: %" PRIu8 "\nAgility: %" PRIu8
           "\nIntelligence: %" PRIu8 "\nStamina: %" PRIu8
           "\nResilience: %" PRIu8 "\nSpirit: %" PRIu8 "\n",
           character.name, get_job_string(character.job),
           character.stats.strength, character.stats.agility,
           character.stats.intelligence, character.stats.stamina,
           character.stats.resilience, character.stats.spirit);
    wait_enter();
}

// Renders the sorting menu
void render_sort() {
    clear_terminal();
    for (int i = 0; i < SORT_COUNT; i++) {
        printf("%d| %s\n", i + 1, get_sort_string(i));
    }
}

// Renders the ordering menu
void render_order() {
    clear_terminal();
    for (int i = 0; i < ORDER_COUNT; i++) {
        printf("%d| %s\n", i + 1, get_order_string(i));
    }
}
