#pragma once

#include "utils/cstd.h"
#include "forward_declaration.h"

// game function
int32_t get_player_move(List *game,Player *bot);

void set_player(List *game,Player *bot);
Player *get_sheriff(List *game);

//stage
void draw_stage(List *game,Player *bot);
