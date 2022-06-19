#pragma once

#include "utils/cstd.h"
#include "forward_declaration.h"

void print_board(List *game,Player *bot);

// print function
void print_card(Card crd);
void print_player_hand(Player *bot);

// for SYS_BAR_PRINT
// mode=0 : read to sys_log, otherwise : print system info
void print_system_msg(int32_t mode,char* str); 
void gotoxy(int32_t x,int32_t y);
void print_card_visual(int32_t x,int32_t y,Card print);
void print_store(int32_t x,int32_t y,int32_t w,int32_t h);
