#pragma once

#include "utils/cstd.h"
#include "forward_declaration.h"
#include "card.h"
#include "list.h"

int32_t available(List *game,Player *bot);

void get_card(Card *new_card,Player *bot);

Player *select_other_player(List *game,Player *bot);
Player *select_range_player(List *game,Player *bot,int32_t range);

//card ability
int32_t play_specify_card(List *game,Player *bot,int32_t card_id);
Card *discard(Player *bot);

int32_t gear_check(Player *bot,int32_t gear_ID);

Card *discard_gear(Player *bot,int32_t gear_ID);

// old version code below
// ==============================
// typedef struct _sPlayerData
// {
//     uint8_t hp;
//     uint8_t role; //size: 4~7
//     uint8_t character; //size: 16
//     int8_t *distance;
//     Card equip;
//     Card card;
//     char *nickname;
// }PlayerData;
// ==============================