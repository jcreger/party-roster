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
    MAX_PARTY_SIZE = 4,
    DELAY_MENU = 1000
};

enum menu_option {
    MENU_ADD,
    MENU_VIEW,
    MENU_CHANGE,
    MENU_REMOVE,
    MENU_QUIT,
    MENU_COUNT
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
            return "JOB UNDEFINED";
    }
}

static const char *get_menu_string(const enum menu_option option_id) {
    switch (option_id) {
        case MENU_ADD:
            return "Add a character";
        case MENU_VIEW:
            return "View party";
        case MENU_CHANGE:
            return "Change stats";
        case MENU_QUIT:
            return "Quit";
        case MENU_REMOVE:
            return "Remove a character";
        default:
            return "MENU UNDEFINED";
    }
}

// Bad implementation of delay that doesn't require specific system headers
void delay(const int milliseconds) {
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < milliseconds)
        ;
}

// Cleans user input and will return status
int clean_input(char string[], const size_t string_size) {
    assert(string_size >= 1);
    if (fgets(string, string_size, stdin) != NULL) {
        if (strchr(string, '\n') == NULL) {
            int c = getchar();
            if (c != '\n' && c != EOF) {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
                memset(string, '\0', string_size);
                return STATUS_LONG_INPUT;
            } else {
                return STATUS_OKAY;
            }
        } else {
            string[strcspn(string, "\n")] = '\0';
            return STATUS_OKAY;
        }
    } else {
        return STATUS_NULL_INPUT;
    }
}

// Cleans user input and converts string to integer return status
enum status clean_input_int(int *num, int digit_count) {
    char string[digit_count + 1];
    size_t string_size = sizeof(string);
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
        if (atoi(string) != 0) {
            *num = atoi(string);
            return STATUS_OKAY;
        } else {
            return STATUS_INVALID_INPUT;
        }
    } else {
        return STATUS_INVALID_INPUT;
    }
}

// Waits until user inputs anything to continue
void wait_enter() {
    char input;
    printf("\npress enter to continue...\n");
    clean_input(&input, sizeof(input));
}

// Print user error messages
void read_status(enum status status_id) {
    clear_terminal();
    switch (status_id) {
        case STATUS_NULL_INPUT:
            printf("You cannot input NULL\n");
            break;
        case STATUS_LONG_INPUT:
            printf("Input too long\n");
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
        case STATUS_EMPTY:
            printf("Party is empty\n");
            break;
        case STATUS_INVALID_OPTION:
            printf("Not an option");
            break;
        default:
            printf("STATUS UNDEFINED");
    }
    wait_enter();
}

// Render the main menu and return valid user input
enum menu_option render_menu(const size_t party_size) {
    int input;
    enum status input_status;

    while (true) {
        clear_terminal();
        printf("%zu/4 Characters\n\n", party_size);
        for (int i = 0; i < MENU_COUNT; i++) {
            printf("%d| %s\n", i + 1, get_menu_string(i));
        }
        input_status = clean_input_int(&input, 1);
        if (input_status == STATUS_OKAY) {
            input--;
            if (input < MENU_COUNT && input >= 0) {
                return input;
            } else {
                input_status = STATUS_INVALID_OPTION;
                read_status(input_status);
            }
        } else {
            read_status(input_status);
        }
    }
}

// Creates a new character struct in party
void add_character(struct character party[], size_t *party_size) {
    char name[CHARACTER_NAME_LENGTH];
    enum status input_status;
    enum status name_status;
    enum status job_status;
    int job_id;
    int index = *party_size;

    if (*party_size < MAX_PARTY_SIZE) {
        while (name_status != STATUS_OKAY) {
            clear_terminal();
            printf("Name: ");
            name_status = clean_input(name, sizeof(name));
            if (name_status == STATUS_OKAY) {
                strcpy(party[index].name, name);
            } else {
                read_status(name_status);
            }
        }
        while (job_status != STATUS_OKAY) {
            clear_terminal();
            for (int i = 0; i < JOB_COUNT; i++) {
                printf("%d| %s\n", i + 1, get_job_string(i));
            }
            job_status = clean_input_int(&job_id, 1);
            if (job_status == STATUS_OKAY) {
                job_id--;
                if (job_id < JOB_COUNT && job_id >= 0) {
                    party[index].job = job_id;
                    party[index].stats = stats_table[job_id];
                } else {
                    job_status = STATUS_INVALID_OPTION;
                    read_status(job_status);
                }
            } else {
                read_status(job_status);
            }
        }
        (*party_size)++;
        clear_terminal();
        printf(
            "Name: %s\n\nJob: %s\n\nStrength: %u\nAgility: %u\nIntelligence: "
            "%u\nStamina: %u\nResilience: %u\nSpirit: %u\n",
            name, get_job_string(job_id), party[index].stats.strength,
            party[index].stats.agility, party[index].stats.intelligence,
            party[index].stats.stamina, party[index].stats.resilience,
            party[index].stats.spirit);
        wait_enter();
    } else {
        input_status = STATUS_LIST_FULL;
        read_status(input_status);
    }
}

void remove_character(struct character party[], size_t party_size) {
    enum status input_status;
    int input;

    clear_terminal();
    if (party_size > 0) {
        for (size_t i = 0; i < party_size; i++) {
            printf("%zu| %s\n", i + 1, party[i].name);
        }
        printf("Remove: ");
        input_status = clean_input_int(&input, 1);
        if (input_status == STATUS_OKAY) {

        } else {
            read_status(input_status);
        }
    } else {
        input_status = STATUS_EMPTY;
        read_status(input_status);
    }
}

int main(void) {
    struct character party[MAX_PARTY_SIZE];
    size_t party_size = 0;
    static bool running = true;
    while (running) {
        enum menu_option user_input = render_menu(party_size);
        switch (user_input) {
            case MENU_ADD:
                printf("MENU_ADD\n");
                add_character(party, &party_size);
                break;
            case MENU_VIEW:
                printf("MENU_VIEW\n");
                break;
            case MENU_CHANGE:
                printf("MENU_CHANGE\n");
                break;
            case MENU_QUIT:
                printf("MENU_QUIT\n");
                running = false;
                break;
            case MENU_REMOVE:
                printf("MENU_REMOVE\n");
                remove_character(party, party_size);
                break;
            default:
                printf("Undefined\n");
                return 1;
        }
    }
    return 0;
}
