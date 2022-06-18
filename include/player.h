#pragma once

#include "utils/cstd.h"
#include "forward_declaration.h"

Player *select_other_player(List *game,Player *bot);
Player *select_range_player(List *game,Player *bot,int32_t range);

void get_card(Card *new_card,Player *bot);
Card *draw(List *game);

//gear
Card *discard_gear(List *game,Player *bot,int32_t gear_ID);

Card *select_discard(List *game,Player *bot);
int32_t get_temp_player_play(List *game,Player *bot);
