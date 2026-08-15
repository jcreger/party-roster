#include "item.h"
#include "render.h"
#include "types.h"
#include <stdio.h>

struct item_definition item_table[SIZE_ITEM] = {

    [ITEM_SWORD_COPPER] = {.name = "Copper Sword",
                           .value = 50,
                           .type = TYPE_WEAPON,
                           .data.weapon.attack_power = 10,
                           .data.weapon.attack_speed = 1},

    [ITEM_CHEST_LEATHER] = {.name = "Leather Chest",
                            .value = 40,
                            .type = TYPE_ARMOR,
                            .data.armor.defense = 4,
                            .data.armor.slot = SLOT_CHEST},

    [ITEM_POTION] = {.name = "Potion",
                     .value = 10,
                     .type = TYPE_RECOVERY,
                     .data.recovery.heal_amount = 20},

    [ITEM_KEY] = {.name = "Key",
                  .value = 0,
                  .type = TYPE_QUEST,
                  .data.quest.quest_id = 0}};

void print_item(const struct item_definition *item) {
    printf("NAME: %s\nVALUE: %d\n", item->name, item->value);
    switch (item->type) {
    case TYPE_WEAPON:
        printf("TYPE: WEAPON\nDAMAGE: %d\nATKSPD: %d\n",
               item->data.weapon.attack_power, item->data.weapon.attack_speed);
        break;
    case TYPE_ARMOR:
        printf("TYPE: ARMOR\nDEFENSE: %d\nSLOT: %s\n", item->data.armor.defense,
               get_slot_string(item->data.armor.slot));
        break;
    case TYPE_RECOVERY:
        printf("TYPE: USE\nHEAL: %d\n", item->data.recovery.heal_amount);
        break;
    case TYPE_QUEST:
        printf("TYPE: KEYITEM\nID: %d\n", item->data.quest.quest_id);
        break;
    default:
        printf("ITEM UNDEFINED");
        break;
    }
}
