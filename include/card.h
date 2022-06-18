#pragma once

#include "utils/cstd.h"
#include "forward_declaration.h"

int32_t gear_check(Player *bot,int32_t gear_ID);

void damg(List *game,Player *bot);

int32_t Dynamite(List *game,Player *bot);

// pile function
void build_pile(List *game);

// check discard pile need to transfer to pile and shuffle
void pile_remain_manage(List *game);