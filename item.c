#include "item.h"
#include "types.h"
#include <stdio.h>

struct item_definition item_table[MAX_ITEM_COUNT] = {
    [ITEM_SWORD_COPPER] = {
        .name = "Copper Sword",
        .value = 50,
        .type = ITEM_TYPE_WEAPON,
        .data.weapon = {.attack_power = 10, .attack_speed = 1}}};

void print_item(const struct item_definition *item) {
    printf("\n\nNAME: %s\nVALUE: %d\n", item->name, item->value);
    switch (item->type) {
    case ITEM_TYPE_WEAPON:
        printf("TYPE: WEAPON\n%d DAMAGE\n%d ATTACK SPEED\n",
               item->data.weapon.attack_power, item->data.weapon.attack_speed);
        break;
    default:
        printf("ERROR");
        break;
    }
}
