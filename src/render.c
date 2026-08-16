#include "input.h"
#include "types.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// returns job string literal to be used in printf
const char *get_job_string(const job job_id) {
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
const char *get_menu_string(const menu_option option_id) {
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
    case MENU_INVENTORY:
        return "Open inventory";
    case MENU_ITEM_ADD:
        return "Add item";
    case MENU_QUIT:
        return "Quit";
    default:
        return "MENU UNDEFINED";
    }
}

// returns sort string literal to be used in printf
const char *get_sort_string(const sort sort_id) {
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
const char *get_order_string(const order order_id) {
    switch (order_id) {
    case ORDER_ASCENDING:
        return "Order by Ascending";
    case ORDER_DESCENDING:
        return "Order by Descending";
    default:
        return "ORDER UNDEFINED";
    }
}

const char *get_slot_string(const armor_slot slot_id) {
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

const char *get_type_string(const item_type type_id) {
    switch (type_id) {
    case TYPE_WEAPON:
        return "WEAPON";
    case TYPE_ARMOR:
        return "ARMOR";
    case TYPE_RECOVERY:
        return "RECOVERY";
    case TYPE_QUEST:
        return "QUEST";
    default:
        return "TYPE UNDEFINED";
    }
}

void clear_terminal(void) {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

// Print user error messages
void read_status(status status_id) {
    clear_terminal();
    switch (status_id) {
    case STATUS_NULL_INPUT:
        printf("You cannot input NULL");
        break;
    case STATUS_LONG_INPUT:
        printf("Input too long");
        break;
    case STATUS_INVALID_INPUT:
        printf("Invalid Input");
        break;
    case STATUS_LIST_FULL:
        printf("List Full");
        break;
    case STATUS_NOT_FOUND:
        printf("Not found");
        break;
    case STATUS_EMPTY:
        printf("Party is empty");
        break;
    case STATUS_INVALID_OPTION:
        printf("Not an option");
        break;
    case STATUS_INV_EMPTY:
        printf("Inventory is empty");
        break;
    case STATUS_Q:
        printf("Q is selected");
        break;
    default:
        break;
    }
    wait_enter();
}

// Renders a menu for the entire party
void render_party(const character party[], size_t party_size) {
    clear_terminal();
    for (size_t i = 0; i < party_size; i++) {
        printf("%zu| %s\n", i + 1, party[i].name);
    }
}

// Renders the quit menu
void render_back(void) {
    printf("\nq| Back\n\n");
    printf("> ");
}

// Render the main menu and return valid user input, returns the user input
menu_option render_menu(const size_t party_size) {
    int input;
    status input_status;
    while (true) {
        clear_terminal();
        printf("%zu/%d Characters\n\n", party_size, MAX_PARTY);
        for (int i = 0; i < MENU_COUNT - 1; i++) {
            printf("%d| %s\n", i + 1, get_menu_string(i));
        }
        printf("\nq| Quit\n\n> ");
        input_status = clean_input_int(&input, 1);
        switch (validate_input(&input, input_status, MENU_COUNT - 1)) {
        case SELECTION_VALID:
            return input;
        case SELECTION_BACK:
            return MENU_QUIT;
            read_status(STATUS_INVALID_OPTION);
            break;
        case SELECTION_INVALID:
            break;
        }
    }
}

// Renders information of a character
void render_party_character(const character character) {
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
void render_sort(void) {
    clear_terminal();
    for (int i = 0; i < SORT_COUNT; i++) {
        printf("%d| %s\n", i + 1, get_sort_string(i));
    }
}

// Renders the ordering menu
void render_order(void) {
    clear_terminal();
    for (int i = 0; i < ORDER_COUNT; i++) {
        printf("%d| %s\n", i + 1, get_order_string(i));
    }
}

void print_item(const item_definition item) {
    printf("NAME: %s\nTYPE: %s\nVALUE: %d\n", item.name,
           get_type_string(item.type), item.value);
    switch (item.type) {
    case TYPE_WEAPON:
        printf("DAMAGE: %d\nATKSPD: %d\n", item.data.weapon.attack_power,
               item.data.weapon.attack_speed);
        break;
    case TYPE_ARMOR:
        printf("DEFENSE: %d\nSLOT: %s\n", item.data.armor.defense,
               get_slot_string(item.data.armor.slot));
        break;
    case TYPE_RECOVERY:
        printf("HEAL: %d\n", item.data.recovery.heal_amount);
        break;
    case TYPE_QUEST:
        printf("ID: %d\n", item.data.quest.quest_id);
        break;
    default:
        printf("ITEM UNDEFINED\n");
        break;
    }
}

void print_inventory(const item_instance inventory[],
                     const size_t inventory_size) {
    clear_terminal();
    if (inventory_size <= 0) {
        read_status(STATUS_INV_EMPTY);
        return;
    }

    for (size_t i = 0; i < inventory_size; i++) {
        print_item(item_table[inventory[i].item_id]);
        printf("ITEM_ID: %d\nQUANT: %d\n\n", inventory[i].item_id,
               inventory[i].quantity);
    }
    wait_enter();
}

void render_item_table(void) {
    clear_terminal();
    for (int i = 0; i < ITEM_COUNT; i++) {
        printf("%d| %s\n", i + 1, item_table[i].name);
    }
}
