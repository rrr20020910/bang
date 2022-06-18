#pragma once

#include "utils/cstd.h"
#include "forward_declaration.h"

void print_board(List *game,Player *bot);

// print function
void print_card(Card crd);
void print_player_hand(Player *bot);

void print_system_msg(int32_t mode,char* str); //mode=0 : read to sys_log, otherwise : print system info
