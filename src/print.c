#include "types.h"

#include <inttypes.h>
#include <stdio.h.>
#include <stdlib.h>

// clear terminal with OS specific system call
void clear_terminal(void) {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

// return string literal of job id
const char *get_job_string(const job_e job_id) {
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
        return "ERROR: JOB STRING UNDEFINED";
    }
}

// return string literal of option id
const char *get_menu_string(const menu_main_e option_id) {
    switch (option_id) {
    case MENU_ADD:
        return "Add Character";
    case MENU_VIEW:
        return "View Party";
    case MENU_CHANGE:
        return "Change Job";
    case MENU_REMOVE:
        return "Remove Character";
    case MENU_SORT:
        return "Sort Characters";
    case MENU_INVENTORY:
        return "Open Inventory";
    case MENU_ITEM_ADD:
        return "Add Item";
    case MENU_ITEM_REMOVE:
        return "Remove Item";
    case MENU_QUIT:
        return "Quit";
    default:
        return "ERROR: MENU STRING UNDEFINED";
    }
}

// return string literal of sort id
const char *get_sort_string(const sort_e sort_id) {
    switch (sort_id) {
    case SORT_NAME:
        return "Sort Name";
    case SORT_STRENGTH:
        return "Sort Strength";
    case SORT_AGILITY:
        return "Sort Agility";
    case SORT_INTELLIGENCE:
        return "Sort Intelligence";
    case SORT_STAMINA:
        return "Sort Stamina";
    case SORT_RESILIENCE:
        return "Sort Resilience";
    case SORT_SPIRIT:
        return "Sort Spirit";
    default:
        return "ERROR: SORT STRING UNDEFINED";
    }
}

// return string literal of order id
const char *get_order_string(const order_e order_id) {
    switch (order_id) {
    case ORDER_ASCENDING:
        return "Order Ascending";
    case ORDER_DESCENDING:
        return "Order Descending";
    default:
        return "ERROR: ORDER STRING UNDEFINED";
    }
}

// return string literal of slot id
const char *get_slot_string(const armor_slot_e slot_id) {
    switch (slot_id) {
    case SLOT_HEAD:
        return "Head";
    case SLOT_CHEST:
        return "Chest";
    case SLOT_LEGS:
        return "Leg";
    case SLOT_HANDS:
        return "Hands";
    case SLOT_FEET:
        return "Feet";
    default:
        return "ERROR: ARMOR SLOT STRING UNDEFINED";
    }
}

// return string literal of type id
const char *get_type_string(const item_type_e type_id) {
    switch (type_id) {
    case TYPE_WEAPON:
        return "Weapon";
    case TYPE_ARMOR:
        return "Armor";
    case TYPE_RECOVERY:
        return "Recovery";
    case TYPE_QUEST:
        return "Quest";
    default:
        return "ERROR: TYPE STRING UNDEFINED";
    }
}

// print status message
void print_status(const status_e status_id) {
    switch (status_id) {
    case STATUS_NULL_INPUT:
        printf("ERROR: CANNOT INPUT NULL\n");
        break;
    case STATUS_LONG_INPUT:
        printf("ERROR: INPUT TOO LONG\n");
        break;
    case STATUS_INVALID_INPUT:
        printf("ERROR: INVALID INPUT\n");
        break;
    case STATUS_PARTY_FULL:
        printf("ERROR: PARTY FULL\n");
        break;
    case STATUS_NOT_FOUND:
        printf("ERROR: NOT FOUND\n");
        break;
    case STATUS_PARTY_EMPTY:
        printf("ERROR: PARTY EMPTY\n");
        break;
    case STATUS_INVALID_OPTION:
        printf("ERROR: INVALID OPTION\n");
        break;
    case STATUS_INVENTORY_EMPTY:
        printf("ERROR: INVENTORY EMPTY\n");
        break;
    case STATUS_INVENTORY_FULL:
        printf("ERROR: INVENTORY FULL\n");
        break;
    case STATUS_MALLOC_FAIL:
        printf("ERROR: MALLOC MEMORY ALLOCATION FAIL\n");
        break;
    case STATUS_REALLOC_FAIL:
        printf("ERROR: REALLOC MEMORY ALLOCATION FAIL\n");
        break;
    default:
        printf("ERROR: STATUS UNDEFINED\n");
        break;
    }
}

// print party list
void print_party(const character_s party[], const size_t party_size) {
    for (size_t i = 0; i < party_size; i++) {
        printf("%zu | %s\n", i + 1, party[i].name);
    }
}

// print back
void print_back(void) { printf("\nQ | Back\n"); }

// print input carrot
void print_carrot(void) { printf("\n> "); }

// print menu
void print_menu(const size_t party_size) {
    printf("%zu / %d Characters\n\n", party_size, MAX_PARTY);

    for (int i = 0; i < MENU_COUNT - 1; i++) {
        printf("%d | %s\n", i + 1, get_menu_string(i));
    }
    printf("\nQ | Quit\n");
    print_carrot();
}

// print character information
void print_character(const character_s character) {
    printf("Name: %s\n\nJob: %s\n\nStrength: %" PRIu8 "\nAgility: %" PRIu8
           "\nIntelligence: %" PRIu8 "\nStamina: %" PRIu8
           "\nResilience: %" PRIu8 "\nSpirit: %" PRIu8 "\n",
           character.name, get_job_string(character.job),
           character.stats.strength, character.stats.agility,
           character.stats.intelligence, character.stats.stamina,
           character.stats.resilience, character.stats.spirit);
}

// print sorting menu
void print_sort(void) {
    for (int i = 0; i < SORT_COUNT; i++) {
        printf("%d| %s\n", i + 1, get_sort_string(i));
    }
}

// print ordering menu
void print_order(void) {
    for (int i = 0; i < ORDER_COUNT; i++) {
        printf("%d| %s\n", i + 1, get_order_string(i));
    }
}

// print item information
void print_item(const item_definition_s item) {
    printf("Name: %s\nType: %s\nValue: %d\n", item.name,
           get_type_string(item.type), item.value);

    switch (item.type) {
    case TYPE_WEAPON:
        printf("Attack Power: %d\nAttack Speed: %d\n",
               item.data.weapon.attack_power, item.data.weapon.attack_speed);
        break;
    case TYPE_ARMOR:
        printf("Defense: %d\nSslot: %s\n", item.data.armor.defense,
               get_slot_string(item.data.armor.slot));
        break;
    case TYPE_RECOVERY:
        printf("Healing: %d\n", item.data.healing.heal_amount);
        break;
    case TYPE_QUEST:
        printf("Quest ID: %d\n", item.data.quest.quest_id);
        break;
    default:
        printf("ERROR: ITEM TYPE UNDEFINED\n");
        break;
    }
}

// print item inventory
void print_inventory(const item_instance_s inventory[],
                     const size_t inventory_size) {
    for (size_t i = 0; i < inventory_size; i++) {
        printf("%zu | x%d | %s\n", i + 1, inventory[i].quantity,
               item_table[inventory[i].item].name);
    }
}

// print item table
void print_item_table(void) {
    for (int i = 0; i < ITEM_COUNT; i++) {
        printf("%d | %s\n", i + 1, item_table[i].name);
    }
}
