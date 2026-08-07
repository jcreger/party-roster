#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void clear_terminal() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

enum {
    CHARACTER_NAME_LENGTH = 16,
    ITEM_NAME_LENGTH = 32,
    MAX_ITEM_COUNT = 255,
    DELAY_MENU = 1000
};

enum menu_option { MENU_ADD, MENU_VIEW, MENU_CHANGE, MENU_QUIT, MENU_COUNT };

enum status {
    STATUS_OKAY,
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
    JOB_ASSASSIN,
    JOB_COUNT
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

struct stats stats_table[JOB_COUNT] = {
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

void delay(const int milliseconds) {
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < milliseconds)
        ;
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

void read_status(enum status status_id) {
    switch (status_id) {
        case STATUS_NULL_INPUT:
            printf("You cannot input NULL\n");
            break;
        case STATUS_LONG_INPUT:
            printf("Input too long\n");
            break;
        case STATUS_OKAY:
            break;
        case STATUS_INVALID_INPUT:
            printf("Invalid Input\n");
            break;
        case STATUS_LIST_FULL:
            printf("List Full\n");
            break;
        case STATUS_NOT_FOUND:
            printf("Not found\n");
            break;
    }
}

enum menu_option render_menu() {
    while (true) {
        clear_terminal();
        printf("1| Add\n2| View\n3| Change\n4| Quit\n");
        char input[2];
        enum status input_result = clean_input(input, sizeof(input));
        if (input_result == STATUS_OKAY) {
            clear_terminal();
            printf("string: %s\n", input);
            delay(DELAY_MENU);
            enum menu_option user_input = atoi(input) - 1;
            if (user_input < MENU_COUNT && user_input > 0) {
                return user_input;
            } else {
                read_status(STATUS_INVALID_INPUT);
                delay(DELAY_MENU);
            }
        } else {
            clear_terminal();
            read_status(input_result);
            delay(DELAY_MENU);
        }
    }
}

int main(void) {
    struct character party[4];
    size_t party_size = 0;
    static bool running = true;
    while (running) {
        enum menu_option user_input = render_menu();
        printf("Integer is: %d\n", user_input);
        printf("%d is MENU_ADD", MENU_ADD);
        delay(DELAY_MENU);
        switch (user_input) {
            case MENU_ADD:
                printf("MENU_ADD");
                break;
            case MENU_VIEW:
                printf("MENU_VIEW");
                break;
            case MENU_CHANGE:
                printf("MENU_CHANGE");
                break;
            case MENU_QUIT:
                running = false;
                break;
            default:
                printf("Undefined Behavior");
                return 1;
        }
        return 0;
    }
}
