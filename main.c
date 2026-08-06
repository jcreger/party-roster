#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum {
    CHARACTER_NAME_LENGTH = 16,
    ITEM_NAME_LENGTH = 32,
    MAX_ITEM_COUNT = 255,
    MAX_JOB_COUNT = 6
};

enum menu_option { MENU_ADD, MENU_VIEW, MENU_CHANGE, MENU_QUIT = 0 };

enum status {
    STATUS_OKAY = 0,
    STATUS_NOT_FOUND,
    STATUS_INVALID_INPUT,
    STATUS_NULL_INPUT,
    STATUS_LONG_INPUT,
    STATUS_LIST_FULL
};

enum job {
    JOB_FIGHTER,
    JOB_THIEF,
    JOB_MAGE,
    JOB_ROGUE,
    JOB_PALADIN,
    JOB_ASSASSIN
};

enum item_type { ITEM_WEAPON, ITEM_ARMOR, ITEM_RECOVERY, ITEM_KEYITEM };

struct stats {
    uint8_t strength;
    uint8_t agility;
    uint8_t intelligence;
    uint8_t stamina;
    uint8_t resilience;
    uint8_t spirit;
};

struct stats stats_table[MAX_JOB_COUNT] = {
    [JOB_FIGHTER] = {.strength = 20,
                     .agility = 10,
                     .intelligence = 5,
                     .stamina = 15,
                     .resilience = 10,
                     .spirit = 5},
    [JOB_THIEF] = {.strength = 10,
                   .agility = 20,
                   .intelligence = 5,
                   .stamina = 10,
                   .resilience = 15,
                   .spirit = 5},
    [JOB_MAGE] = {.strength = 5,
                  .agility = 5,
                  .intelligence = 20,
                  .stamina = 10,
                  .resilience = 10,
                  .spirit = 15},
    [JOB_ROGUE] = {.strength = 15,
                   .agility = 20,
                   .intelligence = 5,
                   .stamina = 10,
                   .resilience = 10,
                   .spirit = 5},
    [JOB_PALADIN] = {.strength = 15,
                     .agility = 5,
                     .intelligence = 10,
                     .stamina = 15,
                     .resilience = 10,
                     .spirit = 10},
    [JOB_ASSASSIN] = {.strength = 10,
                      .agility = 15,
                      .intelligence = 15,
                      .stamina = 10,
                      .resilience = 10,
                      .spirit = 5},
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

int clean_input(char string[], const size_t string_size) {
    assert(string_size >= 1);

    if (fgets(string, string_size, stdin) == NULL) {
        return STATUS_NULL_INPUT;
    }

    if (strchr(string, '\n') == NULL) {
        int c = getchar();

        if (c != '\n' && c != EOF) {
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            memset(string, '\0', string_size);
            return STATUS_LONG_INPUT;
        }
    } else {
        string[strcspn(string, "\n")] = '\0';
    }
    return STATUS_OKAY;
}

int main(void) {
    struct character party[4];
    size_t party_size = 0;
    bool running = true;

    char player[CHARACTER_NAME_LENGTH];

    int input = clean_input(player, sizeof(player));
    switch (input) {
        case STATUS_NULL_INPUT:
            printf("You cannot input NULL\n");
            break;
        case STATUS_LONG_INPUT:
            printf("Input too long\n");
            break;
        case STATUS_OKAY:
            printf("%s\n", player);
            break;
        default:
            printf("CRITICAL ERROR\n");
            break;
    }

    printf("Player is: %zu bytes.\n", sizeof(player));
    printf("Output: %s\n", player);

    return 0;
}
