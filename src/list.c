#include "utils/cstd.h"
#include "forward_declaration.h"
#include "list.h"
#include "player.h"

void free_list(List *game)
{
    Player *current = game->next;
    Player *temp;
    for(int i=0;i<(game->players_num);i++)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
    return;
}

void build_list(List *game,Player bot)
{    
    Player *new;
    new = (Player*)malloc(sizeof(Player));
    
    *new = bot;

    if((game->next) == NULL)
    {
        game->next = new;
        new->next = new;
        return;
    }
    
    Player *sheriff = game->next;
    Player *current = sheriff;
    
    while(current->next != sheriff)
    {
        current = current->next;
    }
    current->next = new;
    new->next = sheriff;
    return;
}
