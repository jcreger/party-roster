#include "render.h"
#include "types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int MENU_DELAY = 1000;

// Delay function with minimal truncating
void delay(int milliseconds) {
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < milliseconds)
        ;
}

// Cleans user input and will return status
int clean_input(char string[], const size_t string_size) {
    assert(string_size >= 1);
    if (fgets(string, string_size, stdin) == NULL) {
        return STATUS_NULL_INPUT;
    }
    if (string[0] == '\n') {
        return STATUS_INVALID_INPUT;
    }
    if (strchr(string, '\n') == NULL) {
        int c = getchar();
        if (c != '\n' && c != EOF) {
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            memset(string, '\0', string_size);
            return STATUS_LONG_INPUT;
        }
        return STATUS_OKAY;
    } else {
        string[strcspn(string, "\n")] = '\0';
        return STATUS_OKAY;
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
    } else {
        string[strcspn(string, "\n")] = '\0';
    }
    if (string[0] == '0' && string[1] == '\0') {
        *num = atoi(string);
        return STATUS_OKAY;
    }
    if (atoi(string) != 0) {
        *num = atoi(string);
        return STATUS_OKAY;
    }
    return STATUS_INVALID_INPUT;
}

// Validates integer input of a user works for a menu and returns codes for the
// selection
enum selection validate_input(int *input, const enum status input_status,
                              const size_t size) {
    if (input_status != STATUS_OKAY) {
        read_status(input_status);
        return SELECTION_INVALID;
    }
    if (*input == 0) {
        read_status(STATUS_INVALID_OPTION);
        return SELECTION_INVALID;
    }
    (*input)--;
    if (*input >= 0 && (size_t)*input < size) {
        return SELECTION_VALID;
    }
    if (*input >= 0 && (size_t)*input == size) {
        return SELECTION_BACK;
    }
    read_status(STATUS_INVALID_OPTION);
    return SELECTION_INVALID;
}

// Waits until user inputs anything to continue
void wait_enter() {
    char input;
    printf("\npress enter to continue...");
    clean_input(&input, sizeof(input));
}
