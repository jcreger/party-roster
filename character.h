#ifndef CHARACTER_H
#define CHARACTER_H

#include "item.h"
#include "types.h"
#include <stdint.h>
#include <stdio.h>

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
    struct item_instance inventory[25];
    size_t inventory_size;
    struct equipment equipment;
};

extern struct stats stats_table[JOB_COUNT];

void add_character(struct character party[], size_t *party_size);
void remove_character(struct character party[], size_t *party_size);
void view_character(const struct character party[], size_t party_size);
void change_character(struct character party[], const size_t party_size);
void sort_character(struct character party[], size_t party_size);

#endif
