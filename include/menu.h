#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <forward_declaration.h>

void start_of_the_game(List *game,char ***player_name);
void print_menu(List *game,char ***player_name);
void print_rules(List *game,char ***player_name);
void menu_induction_control(List *game,char ***player_name);
void menu_game_settings(List *game,char ***player_name);
