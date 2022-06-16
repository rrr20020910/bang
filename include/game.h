#pragma once

#include "utils/cstd.h"
#include "forward_declaration.h"
#include "player.h"
#include "card.h"
#include "list.h"

// game function
Card *draw(List *game);
void set_player(List *game,Player *bot);
int32_t get_distance(Player *bot1,Player *bot2);
Player *get_sheriff(List *game);

//stage
void draw_stage(List *game,Player *bot);
