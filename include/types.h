#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>

enum {
    LEN_NAME_CHARACTER = 16,
    LEN_NAME_ITEM = 32,
    MAX_ITEM = 255,
    MAX_PARTY = 4,
    MAX_INVENTORY = 50,
    MAX_INPUT = 9,
};

typedef enum item_type {
    TYPE_WEAPON,
    TYPE_ARMOR,
    TYPE_RECOVERY,
    TYPE_QUEST,
} item_type_e;

typedef enum item {
    ITEM_INVALID = -1,
    ITEM_SWORD_COPPER,
    ITEM_CHEST_LEATHER,
    ITEM_POTION,
    ITEM_KEY,
    ITEM_COUNT,
} item_e;

typedef enum armor_slot {
    SLOT_HEAD,
    SLOT_CHEST,
    SLOT_LEGS,
    SLOT_HANDS,
    SLOT_FEET,
} armor_slot_e;

typedef enum job {
    JOB_FIGHTER,
    JOB_THIEF,
    JOB_MAGE,
    JOB_ROGUE,
    JOB_PALADIN,
    JOB_ASSASSIN,
    JOB_COUNT
} job_e;

typedef enum status {
    STATUS_OK,
    STATUS_NOT_FOUND,
    STATUS_INVALID_INPUT,
    STATUS_NULL_INPUT,
    STATUS_LONG_INPUT,
    STATUS_PARTY_FULL,
    STATUS_PARTY_EMPTY,
    STATUS_INVALID_OPTION,
    STATUS_INVENTORY_EMPTY,
    STATUS_Q,
    STATUS_INVENTORY_FULL,
    STATUS_MALLOC_FAIL,
    STATUS_REALLOC_FAIL,
} status_e;

typedef enum input { INPUT_VALID, INPUT_Q, INPUT_ERROR } input_e;

typedef enum menu_main {
    MENU_ADD,
    MENU_VIEW,
    MENU_CHANGE,
    MENU_REMOVE,
    MENU_SORT,
    MENU_INVENTORY,
    MENU_ITEM_ADD,
    MENU_ITEM_REMOVE,
    MENU_QUIT,
    MENU_COUNT,
} menu_main_e;

typedef enum sort {
    SORT_NAME,
    SORT_STRENGTH,
    SORT_AGILITY,
    SORT_INTELLIGENCE,
    SORT_STAMINA,
    SORT_RESILIENCE,
    SORT_SPIRIT,
    SORT_COUNT
} sort_e;

typedef enum order { ORDER_ASCENDING, ORDER_DESCENDING, ORDER_COUNT } order_e;

typedef enum party { PARTY_1, PARTY_2, PARTY_3, PARTY_4 } party_e;

typedef struct weapon_data {
    int attack_power;
    int attack_speed;
} weapon_data_s;

typedef struct armor_data {
    int defense;
    armor_slot_e slot;
} armor_data_s;

typedef struct healing_data {
    int heal_amount;
} healing_data_s;

typedef struct quest_data {
    int quest_id;
} quest_data_s;

typedef struct item_defintion {
    char name[LEN_NAME_ITEM];
    int value;
    item_type_e type;
    union data {
        weapon_data_s weapon;
        armor_data_s armor;
        healing_data_s healing;
        quest_data_s quest;
    } data;
} item_definition_s;

typedef struct item_instance {
    uint8_t item;
    uint8_t quantity;
} item_instance_s;

typedef struct equipment {
    int weapon;
    int head;
    int torso;
    int leg;
    int arm;
    int boot;
} equipment_s;

typedef struct stats {
    uint8_t strength;
    uint8_t agility;
    uint8_t intelligence;
    uint8_t stamina;
    uint8_t resilience;
    uint8_t spirit;
    int hp;
    int attack_power;
    int defense;
    int attack_speed;
} stats_s;

typedef struct character {
    char name[LEN_NAME_CHARACTER];
    job_e job;
    stats_s stats;
    item_instance_s *inventory;
    size_t inventory_size;
    equipment_s equipment;
} character_s;

extern item_definition_s item_table[MAX_ITEM];
extern stats_s stats_table[JOB_COUNT];

#endif
