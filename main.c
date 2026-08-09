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

enum arr_len {
    CHARACTER_NAME_LENGTH = 16,
    ITEM_NAME_LENGTH = 32,
    MAX_ITEM_COUNT = 255,
    MAX_PARTY_SIZE = 4,
};

enum menu_option {
    MENU_ADD,
    MENU_VIEW,
    MENU_CHANGE,
    MENU_REMOVE,
    MENU_SORT,
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

enum selection { SELECTION_VALID, SELECTION_QUIT, SELECTION_INVALID };

enum item_type { ITEM_WEAPON, ITEM_ARMOR, ITEM_RECOVERY, ITEM_KEYITEM };

enum stat {
    STAT_STRENGTH,
    STAT_AGILITY,
    STAT_INTELLIGENCE,
    STAT_STAMINA,
    STAT_RESILIENCE,
    STAT_SPIRIT
};

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

void array_swap(struct character *a, struct character *b) {
    struct character temp = *a;
    *a = *b;
    *b = temp;
}

void array_sort_stats(struct character party[], size_t array_size,
                      enum stat stat) {
    for (size_t i = 0; i < array_size; i++) {
        bool swapped = false;
        for (size_t j = 0; j < array_size - i - 1; j++) {
            uint8_t a, b;
            switch (stat) {
            case STAT_STRENGTH:
                a = party[j].stats.strength;
                b = party[j + 1].stats.strength;
                break;
            case STAT_AGILITY:
                a = party[j].stats.agility;
                b = party[j + 1].stats.agility;
                break;
            case STAT_INTELLIGENCE:
                a = party[j].stats.intelligence;
                b = party[j + 1].stats.intelligence;
                break;
            case STAT_STAMINA:
                a = party[j].stats.stamina;
                b = party[j + 1].stats.stamina;
                break;
            case STAT_RESILIENCE:
                a = party[j].stats.resilience;
                b = party[j + 1].stats.resilience;
                break;
            case STAT_SPIRIT:
                a = party[j].stats.spirit;
                b = party[j + 1].stats.spirit;
                break;
            }
            if (a > b) {
                array_swap(&party[j], &party[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            return;
        }
    }
}

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
    case JOB_COUNT:
        return "JOB UNDEFINED";
    }
    return "JOB UNDEFINED";
}

static const char *get_menu_string(const enum menu_option option_id) {
    switch (option_id) {
    case MENU_ADD:
        return "Add a character";
    case MENU_VIEW:
        return "View party";
    case MENU_CHANGE:
        return "Change job";
    case MENU_QUIT:
        return "Quit";
    case MENU_REMOVE:
        return "Remove a character";
    case MENU_SORT:
        return "Sort characters";
    case MENU_COUNT:
        return "MENU UNDEFINED";
    }
    return "MENU UNDEFINED";
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
    *num = 0;

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
        if (sizeof(string) == 2 && string[0] == '0' && string[1] == '\0') {
            *num = atoi(string);
            return STATUS_OKAY;
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
    printf("\npress enter to continue...\n\n");
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
    case STATUS_OKAY:
        printf("Ok");
        break;
    }
    wait_enter();
}

void render_party(const struct character party[], size_t party_size) {
    clear_terminal();
    for (size_t i = 0; i < party_size; i++) {
        printf("%zu| %s\n", i + 1, party[i].name);
    }
}

void render_quit(const size_t party_size) {
    printf("\n\n%zu| Quit\n\n", party_size + 1);
    printf("> ");
}

enum selection validate_party_input(int *input, const enum status input_status,
                                    const size_t party_size) {
    if (input_status == STATUS_OKAY) {
        (*input)--;
        if ((size_t)*input < party_size && *input >= 0) {
            return SELECTION_VALID;
        } else if ((size_t)*input == party_size) {
            return SELECTION_QUIT;
        } else {
            read_status(STATUS_INVALID_INPUT);
            return SELECTION_INVALID;
        }
    } else {
        read_status(input_status);
        return SELECTION_INVALID;
    }
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
        printf("\n> ");
        input_status = clean_input_int(&input, 1);
        if (input_status == STATUS_OKAY) {
            input--;
            if (input < MENU_COUNT && input >= 0) {
                return input;
            } else {
                read_status(STATUS_INVALID_OPTION);
            }
        } else {
            read_status(input_status);
        }
    }
}

void add_name(struct character *character) {
    char name[CHARACTER_NAME_LENGTH];
    enum status name_status;

    while (true) {
        clear_terminal();
        printf("Name: ");
        name_status = clean_input(name, sizeof(name));
        if (name_status == STATUS_OKAY) {
            strcpy(character->name, name);
            return;
        } else {
            read_status(name_status);
        }
    }
}

void add_job(struct character *character) {
    enum status job_status;
    int job_id;

    while (true) {
        clear_terminal();
        for (int i = 0; i < JOB_COUNT; i++) {
            printf("%d| %s\n", i + 1, get_job_string(i));
        }
        printf("\n> ");
        job_status = clean_input_int(&job_id, 1);
        if (job_status == STATUS_OKAY) {
            job_id--;
            if (job_id < JOB_COUNT && job_id >= 0) {
                character->job = job_id;
                character->stats = stats_table[job_id];
                return;
            } else {
                job_status = STATUS_INVALID_OPTION;
                read_status(job_status);
            }
        } else {
            read_status(job_status);
        }
    }
}

void render_party_character(const struct character character) {
    clear_terminal();
    printf("Name: %s\n\nJob: %s\n\nStrength: %" PRIu8 "\nAgility: %" PRIu8
           "\nIntelligence: %" PRIu8 "\nStamina: %" PRIu8
           "\nResilience: %" PRIu8 "\nSpirit: %" PRIu8 "\n",
           character.name, get_job_string(character.job),
           character.stats.strength, character.stats.agility,
           character.stats.intelligence, character.stats.stamina,
           character.stats.resilience, character.stats.spirit);
    wait_enter();
}

// Creates a new character struct in party
void add_character(struct character party[], size_t *party_size) {
    int index = *party_size;

    if (*party_size < MAX_PARTY_SIZE) {
        add_name(&party[index]);
        add_job(&party[index]);
        (*party_size)++;
        render_party_character(party[index]);
    } else {
        read_status(STATUS_LIST_FULL);
    }
}

void remove_character(struct character party[], size_t *party_size) {
    enum status input_status;
    int input;

    if (*party_size > 0) {
        while (true) {
            render_party(party, *party_size);
            render_quit(*party_size);
            input_status = clean_input_int(&input, 1);
            switch (validate_party_input(&input, input_status, *party_size)) {
            case SELECTION_VALID:
                for (size_t i = input; i < *party_size - 1; i++) {
                    party[i] = party[i + 1];
                }
                (*party_size)--;
                if (*party_size == 0) {
                    return;
                }
                break;
            case SELECTION_QUIT:
                return;
            case SELECTION_INVALID:
                break;
            }
        }
    } else {
        read_status(STATUS_EMPTY);
    }
}

// Opens menu to select invididual characters and view their name, job,
// stats
void view_character(const struct character party[], size_t party_size) {
    int input;
    enum status input_status;

    if (party_size > 0) {
        while (true) {
            render_party(party, party_size);
            render_quit(party_size);
            input_status = clean_input_int(&input, 1);
            switch (validate_party_input(&input, input_status, party_size)) {
            case SELECTION_VALID:
                render_party_character(party[input]);
                break;
            case SELECTION_QUIT:
                return;
            case SELECTION_INVALID:
                break;
            }
        }
    } else {
        read_status(STATUS_EMPTY);
    }
}

void change_job(struct character party[], const size_t party_size) {
    int input;
    enum status input_status;

    if (party_size > 0) {
        while (true) {
            render_party(party, party_size);
            render_quit(party_size);
            input_status = clean_input_int(&input, 1);
            switch (validate_party_input(&input, input_status, party_size)) {
            case SELECTION_VALID:
                add_job(&party[input]);
                render_party_character(party[input]);
                break;
            case SELECTION_QUIT:
                return;
            case SELECTION_INVALID:
                break;
            }
        }
    } else {
        read_status(STATUS_EMPTY);
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
            add_character(party, &party_size);
            break;
        case MENU_VIEW:
            view_character(party, party_size);
            break;
        case MENU_CHANGE:
            change_job(party, party_size);
            break;
        case MENU_QUIT:
            running = false;
            break;
        case MENU_REMOVE:
            remove_character(party, &party_size);
            break;
        case MENU_SORT:
            array_sort_stats(party, party_size, STAT_STRENGTH);
            break;
        default:
            printf("MENU FUCKED\n");
            return 1;
        }
    }
    clear_terminal();
    return 0;
}
