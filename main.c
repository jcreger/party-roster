#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include <strings.h>
#endif

void clear_terminal() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

int string_case_compare(char str1[], char str2[]) {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    return strcasecmp(str1, str2);
#endif
#if defined(_WIN32) || defined(_WIN64)
    return _stricmp(str1, str2);
#endif
}

static const int MENU_DELAY = 1000;

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
        return "Change job";
    case MENU_REMOVE:
        return "Remove a character";
    case MENU_SORT:
        return "Sort characters";
    default:
        return "MENU UNDEFINED";
    }
}

static const char *get_sort_string(const enum sort sort_id) {
    switch (sort_id) {
    case SORT_NAME:
        return "Sort by Name";
    case SORT_STRENGTH:
        return "Sort by Strength";
    case SORT_AGILITY:
        return "Sort by Agility";
    case SORT_INTELLIGENCE:
        return "Sort by Intelligence";
    case SORT_STAMINA:
        return "Sort by Stamina";
    case SORT_RESILIENCE:
        return "Sort by Stamina";
    case SORT_SPIRIT:
        return "Sort by Spirit";
    default:
        return "SORT UNDEFINED";
    }
}

static const char *get_order_string(const enum order order_id) {
    switch (order_id) {
    case ORDER_ASCENDING:
        return "Order by Ascending";
    case ORDER_DESCENDING:
        return "Order by Descending";
    default:
        return "ORDER UNDEFINED";
    }
}

void delay(int milliseconds) {
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < milliseconds)
        ;
}

void struct_swap(struct character *a, struct character *b) {
    struct character temp = *a;
    *a = *b;
    *b = temp;
}

void array_sort(struct character party[], size_t array_size, enum sort sort_id,
                enum order order_id) {
    for (size_t i = 0; i < array_size; i++) {
        bool swapped = false;
        for (size_t j = 0; j < array_size - i - 1; j++) {
            uint8_t a, b;
            char str1[CHARACTER_NAME_LENGTH], str2[CHARACTER_NAME_LENGTH];
            switch (sort_id) {
            case SORT_STRENGTH:
                a = party[j].stats.strength;
                b = party[j + 1].stats.strength;
                break;
            case SORT_AGILITY:
                a = party[j].stats.agility;
                b = party[j + 1].stats.agility;
                break;
            case SORT_INTELLIGENCE:
                a = party[j].stats.intelligence;
                b = party[j + 1].stats.intelligence;
                break;
            case SORT_STAMINA:
                a = party[j].stats.stamina;
                b = party[j + 1].stats.stamina;
                break;
            case SORT_RESILIENCE:
                a = party[j].stats.resilience;
                b = party[j + 1].stats.resilience;
                break;
            case SORT_SPIRIT:
                a = party[j].stats.spirit;
                b = party[j + 1].stats.spirit;
                break;
            case SORT_NAME:
                strcpy(str1, party[j].name);
                strcpy(str2, party[j + 1].name);
                break;
            case SORT_COUNT:
                clear_terminal();
                printf("%s", get_sort_string(SORT_COUNT));
                break;
            }
            if (sort_id != SORT_NAME) {
                switch (order_id) {
                case ORDER_ASCENDING:
                    if (a > b) {
                        struct_swap(&party[j], &party[j + 1]);
                        swapped = true;
                    }
                    break;
                case ORDER_DESCENDING:
                    if (a < b) {
                        struct_swap(&party[j], &party[j + 1]);
                        swapped = true;
                    }
                    break;
                case ORDER_COUNT:
                    clear_terminal();
                    printf("%s", get_order_string(ORDER_COUNT));
                    break;
                }
            } else {
                switch (order_id) {
                case ORDER_ASCENDING:
                    if (string_case_compare(str1, str2) > 0) {
                        struct_swap(&party[j], &party[j + 1]);
                        swapped = true;
                    }
                    break;
                case ORDER_DESCENDING:
                    if (string_case_compare(str1, str2) < 0) {
                        struct_swap(&party[j], &party[j + 1]);
                        swapped = true;
                    }
                    break;
                case ORDER_COUNT:
                    clear_terminal();
                    printf("%s", get_order_string(ORDER_COUNT));
                    break;
                }
            }
        }
        if (!swapped) {
            return;
        }
    }
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
    printf("\npress enter to continue...");
    clean_input(&input, sizeof(input));
}

// Print user error messages
void read_status(enum status status_id) {
    clear_terminal();
    switch (status_id) {
    case STATUS_NULL_INPUT:
        printf("You cannot input NULL");
        break;
    case STATUS_LONG_INPUT:
        printf("Input too long");
        break;
    case STATUS_INVALID_INPUT:
        printf("Invalid Input");
        break;
    case STATUS_LIST_FULL:
        printf("List Full");
        break;
    case STATUS_NOT_FOUND:
        printf("Not found");
        break;
    case STATUS_EMPTY:
        printf("Party is empty");
        break;
    case STATUS_INVALID_OPTION:
        printf("Not an option");
        break;
    default:
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

void render_quit(const size_t count) {
    printf("\n%zu| Quit\n\n", count + 1);
    printf("> ");
}

enum selection validate_input(int *input, const enum status input_status,
                              const size_t count) {
    if (input_status == STATUS_OKAY) {
        (*input)--;
        if ((size_t)*input < count && *input >= 0) {
            return SELECTION_VALID;
        } else if ((size_t)*input == count) {
            return SELECTION_QUIT;
        } else {
            read_status(STATUS_INVALID_OPTION);
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
        render_quit(MENU_COUNT);
        input_status = clean_input_int(&input, 1);
        switch (validate_input(&input, input_status, MENU_COUNT)) {
        case SELECTION_VALID:
            return input;
        case SELECTION_QUIT:
            return MENU_COUNT;
        case SELECTION_INVALID:
            break;
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
            switch (validate_input(&input, input_status, *party_size)) {
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

/* Opens menu to select invididual characters and view their name, job,
 stats */
void view_character(const struct character party[], size_t party_size) {
    int input;
    enum status input_status;

    if (party_size > 0) {
        while (true) {
            render_party(party, party_size);
            render_quit(party_size);
            input_status = clean_input_int(&input, 1);
            switch (validate_input(&input, input_status, party_size)) {
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
            switch (validate_input(&input, input_status, party_size)) {
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

void render_sort() {
    clear_terminal();
    for (int i = 0; i < SORT_COUNT; i++) {
        printf("%d| %s\n", i + 1, get_sort_string(i));
    }
}

void render_order() {
    clear_terminal();
    for (int i = 0; i < ORDER_COUNT; i++) {
        printf("%d| %s\n", i + 1, get_order_string(i));
    }
}

void sort_character(struct character party[], size_t party_size) {
    enum status sort_status, order_status;
    int sort, order;
    if (party_size > 0) {
        while (true) {
            render_sort();
            render_quit(SORT_COUNT);
            sort_status = clean_input_int(&sort, 1);
            switch (validate_input(&sort, sort_status, SORT_COUNT)) {
            case SELECTION_VALID:
                while (true) {
                    render_order();
                    printf("\n> ");
                    order_status = clean_input_int(&order, 1);
                    switch (validate_input(&order, order_status, ORDER_COUNT)) {
                    case SELECTION_VALID:
                        array_sort(party, party_size, sort, order);
                        render_party(party, party_size);
                        wait_enter();
                        return;
                    case SELECTION_QUIT:
                        read_status(STATUS_INVALID_OPTION);
                        break;
                    case SELECTION_INVALID:
                        break;
                    }
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
        case MENU_REMOVE:
            remove_character(party, &party_size);
            break;
        case MENU_SORT:
            sort_character(party, party_size);
            break;
        default:
            running = false;
            clear_terminal();
            printf("Quitting...");
            delay(MENU_DELAY);
            clear_terminal();
            return 0;
        }
    }
}
