#include "utils/cstd.h"
#include"game.h"
#include "card.h"
#include "list.h"
#include "player.h"

// draw            ||     
//=================\/===
Card *draw(List *game)
{
    int32_t ori_pos = game->pos;
    game->pos++;
    return &game->pile[ori_pos];
}

// set_player      ||     
//=================\/===
void set_player(List *game,Player *bot)
{
    printf("please enter user name : ");
    char name[16];
    scanf("%s",name);
    strcpy(bot->user_name,name);

    bot->charater_ID = 401;
    bot->bullets = (bot->charater_ID)/100;
    
    for(int i=0;i<(bot->bullets);i++)
    {
        get_card(draw(game),bot);
    }
    
    return;
}

// distance        ||     
//=================\/===
// range == 0 means two sides
static int32_t compare(int32_t a,int32_t b)
{
    if(a > b) return b;
    return a;
}

int32_t get_distance(Player *bot1,Player *bot2)
{
    int32_t left = 0;
    int32_t right = 0;
    Player *current = bot1;
    
    while(current != bot2)
    {
        current = current->next;
        left++;
    }

    while(current != bot1)
    {
        current = current->next;
        right++;
    }
    return compare(right,left);
}

// sheriff         ||     
//=================\/===
Player *get_sheriff(List *game)
{    
    Player *sheriff = game->next;
    
    while(sheriff->role_ID != SHERIFF)
    {
        sheriff = sheriff->next;
    }
    printf("bugger1\n");
    
    return sheriff;
}

// Stage           ||     
//=================\/=== 
void draw_stage(List *game,Player *bot)
{
    game->card_page = 0;
    
    if(gear_check(bot,JAIL))
    {
        bot = bot->next;
        discard_gear(bot,JAIL);
    }
    
    if(gear_check(bot,DYNAMITE))
    {
        damg(game,bot);
        discard_gear(bot,DYNAMITE);
        Dynamite(game,bot->next);
    }
    
    get_card(draw(game),bot);
    get_card(draw(game),bot);
}
