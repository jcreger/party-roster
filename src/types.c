#include "types.h"

item_definition item_table[MAX_ITEM] = {
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

stats stats_table[JOB_COUNT] = {

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
