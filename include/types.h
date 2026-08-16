#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

enum {
    LEN_NAME_CHARACTER = 16,
    LEN_NAME_ITEM = 32,
    MAX_ITEM = 255,
    MAX_PARTY = 4,
    MAX_INVENTORY = 50,
};

typedef enum {
    TYPE_WEAPON,
    TYPE_ARMOR,
    TYPE_RECOVERY,
    TYPE_QUEST,
} item_type;

typedef enum {
    ITEM_SWORD_COPPER,
    ITEM_CHEST_LEATHER,
    ITEM_POTION,
    ITEM_KEY,
    ITEM_COUNT,
} item_id;

typedef enum {
    SLOT_HEAD,
    SLOT_CHEST,
    SLOT_LEG,
    SLOT_GLOVE,
    SLOT_BOOT,
} armor_slot;

typedef enum {
    JOB_FIGHTER,
    JOB_THIEF,
    JOB_MAGE,
    JOB_ROGUE,
    JOB_PALADIN,
    JOB_ASSASSIN,
    JOB_COUNT
} job;

typedef enum {
    STATUS_OKAY,
    STATUS_NOT_FOUND,
    STATUS_INVALID_INPUT,
    STATUS_NULL_INPUT,
    STATUS_LONG_INPUT,
    STATUS_LIST_FULL,
    STATUS_EMPTY,
    STATUS_INVALID_OPTION,
    STATUS_INV_EMPTY,
    STATUS_Q,
} status;

typedef enum { SELECTION_VALID, SELECTION_BACK, SELECTION_INVALID } selection;

typedef enum {
    MENU_ADD,
    MENU_VIEW,
    MENU_CHANGE,
    MENU_REMOVE,
    MENU_SORT,
    MENU_INVENTORY,
    MENU_ITEM_ADD,
    MENU_QUIT,
    MENU_COUNT,
} menu_option;

typedef enum {
    SORT_NAME,
    SORT_STRENGTH,
    SORT_AGILITY,
    SORT_INTELLIGENCE,
    SORT_STAMINA,
    SORT_RESILIENCE,
    SORT_SPIRIT,
    SORT_COUNT
} sort;

typedef enum { ORDER_ASCENDING, ORDER_DESCENDING, ORDER_COUNT } order;

typedef enum { PARTY_1, PARTY_2, PARTY_3, PARTY_4 } party_id;

typedef struct {
    int attack_power;
    int attack_speed;
} weapon_data;

typedef struct {
    int defense;
    armor_slot slot;
} armor_data;

typedef struct {
    int heal_amount;
} recovery_data;

typedef struct {
    int quest_id;
} quest_data;

typedef struct {
    char name[LEN_NAME_ITEM];
    int value;
    item_type type;
    union data {
        weapon_data weapon;
        armor_data armor;
        recovery_data recovery;
        quest_data quest;
    } data;
} item_definition;

typedef struct {
    uint8_t item_id;
    uint8_t quantity;
} item_instance;

typedef struct {
    int weapon;
    int head;
    int torso;
    int leg;
    int arm;
    int boot;
} equipment;

typedef struct {
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
} stats;

typedef struct {
    char name[LEN_NAME_CHARACTER];
    job job;
    stats stats;
    item_instance inventory[MAX_INVENTORY];
    size_t inventory_size;
    equipment equipment;
} character;

extern item_definition item_table[MAX_ITEM];
extern stats stats_table[JOB_COUNT];

#endif
