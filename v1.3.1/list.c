#include<stdio.h>
#include<stdlib.h>
#include"game.h"

void free_list(list *game){
    player *current = game->next;
    player *temp;
    for(int i=0;i<(game->players_num);i++){
        temp = current;
        current = current->next;
        free(temp);
    }
    return;
}

void build_list(list *game,player bot){
    
    player *new;
    new = (player*)malloc(sizeof(player));
    
    *new = bot;

    if((game->next) == NULL){
        game->next = new;
        new->next = new;
        return;
    }
    
    player *sheriff = game->next;
    player *current = sheriff;
    
    while(current->next != sheriff){
        current = current->next;
    }
    current->next = new;
    new->next = sheriff;
    
    return;
}
