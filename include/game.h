#pragma once

#include "utils/cstd.h"
#include "forward_declaration.h"

// game function
int32_t get_player_move(List *game,Player *bot);

void set_role(List *game);
void set_character(List *game);
Player *get_sheriff(List *game);

//stage
void draw_stage(List *game,Player *bot);
void discard_stage(List *game,Player *bot);

void win_message(List game);
