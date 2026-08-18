#include "types.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int MENU_DELAY = 1000;

// delay function
void delay(int milliseconds) {
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < milliseconds)
        ;
}

// return status, flush stdin buffer, strip '\n'
// due to the termination character the size is effectively -1
status_e clean_input_string(char string[], const size_t string_size) {
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
        } else {
            return STATUS_OK;
        }
    } else {
        string[strcspn(string, "\n")] = '\0';
        return STATUS_OK;
    }
}

// return status, flush stdin buffer, strip '\n', convert to int
// due to the termination character the size is effectively -1
status_e clean_input_int(int *num) {
    char buffer[MAX_INPUT];
    size_t buffer_size = sizeof(buffer);

    *num = -1;

    if (fgets(buffer, buffer_size, stdin) == NULL) {
        return STATUS_NULL_INPUT;
    }
    if (buffer[0] == '\n') {
        return STATUS_INVALID_INPUT;
    }

    if (strchr(buffer, '\n') == NULL) {
        int c = getchar();
        if (c != '\n' && c != EOF) {
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            memset(buffer, '\0', buffer_size);
            return STATUS_LONG_INPUT;
        }
    } else {
        buffer[strcspn(buffer, "\n")] = '\0';
    }

    if ((buffer[0] == 'q' || buffer[0] == 'Q') && buffer[1] == '\0') {
        return STATUS_Q;
    }
    if (buffer[0] == '0' && buffer[1] == '\0') {
        *num = atoi(buffer);
        return STATUS_OK;
    }

    if (atoi(buffer) != 0) {
        *num = atoi(buffer);
        return STATUS_OK;
    }
    return STATUS_INVALID_INPUT;
}

// validate input works for a menu and return code
input_e validate_input(int *input, status_e *input_status, const size_t count) {
    if (*input_status == STATUS_Q) {
        return INPUT_Q;
    }
    if (*input_status != STATUS_OK) {
        return INPUT_ERROR;
    }
    if (*input == 0) {
        *input_status = STATUS_INVALID_OPTION;
        return INPUT_ERROR;
    }

    (*input)--;
    if (*input >= 0 && (size_t)*input < count) {
        return INPUT_VALID;
    }
    *input_status = STATUS_INVALID_OPTION;
    return INPUT_ERROR;
}

// wait for user input to continue
void wait_enter(void) {
    char input;

    printf("\npress enter to continue...");
    clean_input_string(&input, sizeof(input));
}
