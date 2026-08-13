#include "character.h"
#include "input.h"
#include "render.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

struct item_definition iron_sword = {
    .name = "Iron Sword",
    .value = 50,
    .type = ITEM_WEAPON,
    .data.weapon = {.attack_power = 10, .attack_speed = 1}};

void print_item(const struct item_definition *item) {
    printf("\n\nNAME: %s\nVALUE: %d\n", item->name, item->value);
    switch (item->type) {
    case ITEM_WEAPON:
        printf("TYPE: WEAPON\n%d DAMAGE\n%d ATTACK SPEED\n",
               item->data.weapon.attack_power, item->data.weapon.attack_speed);
        break;
    default:
        printf("ERROR");
        break;
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
            print_item(&iron_sword);
            wait_enter();
            running = false;
            clear_terminal();
            printf("Quitting...");
            delay(MENU_DELAY);
            clear_terminal();
            return 0;
        }
    }
}
