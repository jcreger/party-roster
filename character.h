#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>

enum arr_len {
    CHARACTER_NAME_LENGTH = 16,
    ITEM_NAME_LENGTH = 32,
    MAX_ITEM_COUNT = 255,
    MAX_PARTY_SIZE = 4,
};

enum item_type { ITEM_WEAPON, ITEM_ARMOR, ITEM_RECOVERY, ITEM_KEYITEM };

enum job {
    JOB_FIGHTER,
    JOB_THIEF,
    JOB_MAGE,
    JOB_ROGUE,
    JOB_PALADIN,
    JOB_ASSASSIN,
    JOB_COUNT
};

struct stats {
    uint8_t strength;
    uint8_t agility;
    uint8_t intelligence;
    uint8_t stamina;
    uint8_t resilience;
    uint8_t spirit;
};

struct item_definition {
    char name[ITEM_NAME_LENGTH];
    uint16_t value;
    enum item_type item_type;
};

struct item_instance {
    uint8_t item_id;
    uint8_t quantity;
};

struct character {
    char name[CHARACTER_NAME_LENGTH];
    enum job job;
    struct stats stats;
    struct item_instance inventory[50];
    size_t inventory_size;
};

extern struct item_definition item_table[MAX_ITEM_COUNT];
extern struct stats stats_table[JOB_COUNT];

void add_character(struct character party[], size_t *party_size);
void remove_character(struct character party[], size_t *party_size);
void view_character(const struct character party[], size_t party_size);
void change_job(struct character party[], const size_t party_size);
void sort_character(struct character party[], size_t party_size);

#endif
