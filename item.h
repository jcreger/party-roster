#ifndef ITEM_H
#define ITEM_H

#include "types.h"
#include <stdint.h>

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

void print_item(const struct item_definition *item);
extern struct item_definition item_table[SIZE_ITEM];

#endif
