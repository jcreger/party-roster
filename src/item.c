#include "render.h"
#include "types.h"

#include <stdio.h>
#include <string.h>

struct item_definition item_table[MAX_ITEM] = {
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
    printf("NAME: %s\nTYPE: %s\nVALUE: %d\n", item->name,
           get_type_string(item->type), item->value);
    switch (item->type) {
    case TYPE_WEAPON:
        printf("DAMAGE: %d\nATKSPD: %d\n", item->data.weapon.attack_power,
               item->data.weapon.attack_speed);
        break;
    case TYPE_ARMOR:
        printf("DEFENSE: %d\nSLOT: %s\n", item->data.armor.defense,
               get_slot_string(item->data.armor.slot));
        break;
    case TYPE_RECOVERY:
        printf("HEAL: %d\n", item->data.recovery.heal_amount);
        break;
    case TYPE_QUEST:
        printf("ID: %d\n", item->data.quest.quest_id);
        break;
    default:
        printf("ITEM UNDEFINED\n");
        break;
    }
}

void add_item(enum item item_id, struct item_instance inventory[],
              size_t *inventory_size) {
    if (*inventory_size >= MAX_INVENTORY) {
        return;
    }

    for (size_t i = 0; i < *inventory_size; i++) {
        if (inventory[i].item_id == item_id) {
            inventory[i].quantity++;
            return;
        }
    }

    inventory[*inventory_size].item_id = item_id;
    inventory[*inventory_size].quantity = 1;
    (*inventory_size)++;
    return;
}

void remove_item(enum item item_id, struct item_instance inventory[],
                 size_t *inventory_size) {
    if (*inventory_size == 0) {
        return;
    }

    for (size_t i = 0; i < *inventory_size; i++) {
        if (inventory[i].item_id == item_id) {
            inventory[i].quantity--;
            if (inventory[i].quantity == 0) {
                for (size_t j = i; j < *inventory_size - 1; j++) {
                    inventory[j] = inventory[j + 1];
                }
                memset(&inventory[*inventory_size - 1], '\0',
                       sizeof(struct item_instance));
                (*inventory_size)--;
                i--;
            }
            return;
        }
    }

    return;
}

void print_item_instance(const struct character *character) {
    for (size_t i = 0; i < character->inventory_size; i++) {
        print_item(&item_table[character->inventory[i].item_id]);
        printf("ITEM_ID: %d\nQUANT: %d\n\n", character->inventory[i].item_id,
               character->inventory[i].quantity);
    }
}
