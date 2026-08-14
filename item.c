#include "item.h"
#include "render.h"
#include "types.h"
#include <stdio.h>

struct item_definition item_table[MAX_ITEM_COUNT] = {
    [ITEM_SWORD_COPPER] = {.name = "Copper Sword",
                           .value = 50,
                           .type = ITEM_TYPE_WEAPON,
                           .data.weapon = {.attack_power = 10,
                                           .attack_speed = 1}},
    [ITEM_ARMOR_CHEST_LEATHER] = {.name = "Leather Chest",
                                  .value = 40,
                                  .type = ITEM_TYPE_ARMOR,
                                  .data.armor = {.defense = 4,
                                                 .slot = ARMOR_CHEST}},
};

void print_item(const struct item_definition *item) {
    printf("\n\nNAME: %s\nVALUE: %d\n", item->name, item->value);
    switch (item->type) {
    case ITEM_TYPE_WEAPON:
        printf("TYPE: WEAPON\n%d DAMAGE\n%d ATTACK SPEED\n",
               item->data.weapon.attack_power, item->data.weapon.attack_speed);
        break;
    case ITEM_TYPE_ARMOR:
        printf("TYPE: ARMOR\n%d DEFENSE\n%s SLOT\n", item->data.armor.defense,
               get_slot_string(item->data.armor.slot));
        break;
    case ITEM_TYPE_RECOVERY:
        printf("TYPE: RECOVERY\n%d HEALING\n", item->data.recovery.heal_amount);
        break;
    case ITEM_TYPE_KEYITEM:
        printf("TYPE: KEYITEM\n%d QUEST ID\n", item->data.key_item.quest_id);
        break;
    default:
        printf("ITEM UNDEFINED");
        break;
    }
}
