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
        printf("TYPE: WEAPON\nDAMAGE: %d\nATKSPD: %d\n\n",
               item->data.weapon.attack_power, item->data.weapon.attack_speed);
        break;
    case TYPE_ARMOR:
        printf("TYPE: ARMOR\nDEFENSE: %d\nSLOT: %s\n\n",
               item->data.armor.defense,
               get_slot_string(item->data.armor.slot));
        break;
    case TYPE_RECOVERY:
        printf("TYPE: USE\nHEAL: %d\n\n", item->data.recovery.heal_amount);
        break;
    case TYPE_QUEST:
        printf("TYPE: KEYITEM\nID: %d\n\n", item->data.quest.quest_id);
        break;
    default:
        printf("ITEM UNDEFINED");
        break;
    }
}

void add_item(enum item item_id, struct character *character) {
    if (character->inventory_size >= SIZE_INVENTORY) {
        return;
    }
    for (size_t i = 0; i < character->inventory_size; i++) {
        if (character->inventory[i].item_id == item_id) {
            character->inventory[i].quantity += 1;
            return;
        }
    }
    character->inventory[character->inventory_size].item_id = item_id;
    character->inventory[character->inventory_size].quantity = 1;
    character->inventory_size++;
    return;
}

void print_item_instance(const struct character *character) {
    for (size_t i = 0; i < character->inventory_size; i++) {
        printf("ITEM_ID: %d\nQUANT: %d\n", character->inventory[i].item_id,
               character->inventory[i].quantity);
        print_item(&item_table[character->inventory[i].item_id]);
    }
}
