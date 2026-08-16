#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>

enum item_type {
    TYPE_WEAPON,
    TYPE_ARMOR,
    TYPE_RECOVERY,
    TYPE_QUEST,
};

enum item {
    ITEM_SWORD_COPPER,
    ITEM_CHEST_LEATHER,
    ITEM_POTION,
    ITEM_KEY,
};

enum armor_slot {
    SLOT_HEAD,
    SLOT_CHEST,
    SLOT_LEG,
    SLOT_GLOVE,
    SLOT_BOOT,
};
enum {
    LEN_NAME_CHARACTER = 16,
    LEN_NAME_ITEM = 32,
    MAX_ITEM = 255,
    MAX_PARTY = 4,
    MAX_INVENTORY = 50,
};

enum job {
    JOB_FIGHTER,
    JOB_THIEF,
    JOB_MAGE,
    JOB_ROGUE,
    JOB_PALADIN,
    JOB_ASSASSIN,
    JOB_COUNT
};
enum status {
    STATUS_OKAY,
    STATUS_NOT_FOUND,
    STATUS_INVALID_INPUT,
    STATUS_NULL_INPUT,
    STATUS_LONG_INPUT,
    STATUS_LIST_FULL,
    STATUS_EMPTY,
    STATUS_INVALID_OPTION
};

enum selection { SELECTION_VALID, SELECTION_BACK, SELECTION_INVALID };

enum menu_option {
    MENU_ADD,
    MENU_VIEW,
    MENU_CHANGE,
    MENU_REMOVE,
    MENU_SORT,
    MENU_QUIT,
    MENU_COUNT,
};

enum sort {
    SORT_NAME,
    SORT_STRENGTH,
    SORT_AGILITY,
    SORT_INTELLIGENCE,
    SORT_STAMINA,
    SORT_RESILIENCE,
    SORT_SPIRIT,
    SORT_COUNT
};

enum order { ORDER_ASCENDING, ORDER_DESCENDING, ORDER_COUNT };

struct weapon_data {
    int attack_power;
    int attack_speed;
};

struct armor_data {
    int defense;
    enum armor_slot slot;
};

struct recovery_data {
    int heal_amount;
};
struct quest_data {
    int quest_id;
};

struct item_definition {
    char name[LEN_NAME_ITEM];
    int value;
    enum item_type type;
    union data {
        struct weapon_data weapon;
        struct armor_data armor;
        struct recovery_data recovery;
        struct quest_data quest;
    } data;
};

struct item_instance {
    uint8_t item_id;
    uint8_t quantity;
};

struct equipment {
    int weapon;
    int head;
    int torso;
    int leg;
    int arm;
    int boot;
};

struct stats {
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
};

struct character {
    char name[LEN_NAME_CHARACTER];
    enum job job;
    struct stats stats;
    struct item_instance inventory[MAX_INVENTORY];
    size_t inventory_size;
    struct equipment equipment;
};

#endif
