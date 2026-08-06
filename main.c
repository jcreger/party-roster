#include <inttypes.h>
#include <stdio.h>

enum {
    CHARACTER_NAME_LENGTH = 16,
    ITEM_NAME_LENGTH = 32,
    MAX_ITEM_COUNT = 255
};

enum job {
    JOB_FIGHTER,
    JOB_THIEF,
    JOB_MAGE,
    JOB_ROGUE,
    JOB_PALADIN,
    JOB_ASSASSIN
};

enum item_type {
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOR,
    ITEM_TYPE_RECOVERY,
    ITEM_TYPE_KEYITEM
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

struct item_definition item_table[MAX_ITEM_COUNT];

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

// Returns a string literal for the given job's display name
static const char *get_job_string(const enum job job_id) {
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
            return "Unknown";
    }
}

int main(void) {
    struct character party[4];
    size_t party_size = 0;

    return 0;
}
