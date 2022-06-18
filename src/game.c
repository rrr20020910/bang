#include "utils/cstd.h"
#include "game.h"
#include "card.h"
#include "list.h"
#include "player.h"
#include "print.h"

// available move  ||     
//=================\/===
int32_t play_card(List *game,Player *bot,int32_t sel)
{    
    if(sel > bot->cards_num)
    {
        printf("fail play\n");
        return 0;
    }
    
    Card *current = &(bot->hand_card[sel]);
    game->current_card = current;
    
    if(bot->hand_card[sel].ability(game,bot))
    {
        printf("fail play\n");
        return 0;
    }
    
    if((bot->hand_card[sel].card_ID)/100 == 1)
    {
        SYS_BAR_PRINT("%s play %s",bot->user_name,game->current_card->name);
        game->discard_pile[game->discard_pos] = *game->current_card;
        game->discard_pos++;
    }
    
    for(int i=sel;i<(bot->cards_num);i++)
    {
        bot->hand_card[i] = bot->hand_card[i+1];
    }
    bot->cards_num--;
    memset(&(bot->hand_card[bot->cards_num]),0,sizeof(Card));
    return 0;
}

int32_t get_player_move(List *game,Player *bot)
{
    //get select
    printf("please select a move:");
    int32_t sel=0;
    scanf("%d",&sel);
    
    //1~6 play card
    if((sel >= 1) && (sel <= 6))
    {
        play_card(game,bot,sel+(game->card_page)*6-1);
    }
    
    //0,7 page control
    if(sel == 0)
    {
        if(game->card_page == 0)
        {
            printf("you can't do that");
            return 0;
        }
        game->card_page --;
        return 0;
    }
    if(sel == 7)
    {
        game->card_page ++;
        if(game->card_page > 3)
        {
            game->card_page = 0;
        }
    }
    
    //9 leave
    if(sel == 9)
    {
        return 1;
    }
    
    return 0;
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

// sheriff         ||     
//=================\/===
Player *get_sheriff(List *game)
{
    Player *sheriff = game->next;
    while(sheriff->role_ID != SHERIFF)
    {
        sheriff = sheriff->next;
    }
    return sheriff;
}

// Stage           ||     
//=================\/=== 
void draw_stage(List *game,Player *bot)
{
    game->card_page = 0;
    
    if(gear_check(bot,DYNAMITE))
    {
        damg(game,bot);
        
        Card *current = &(bot->gear[0]);
        game->current_card = current;
        
        Dynamite(game,bot->next);
        
        discard_gear(game,bot,DYNAMITE);
    }
    
    if(gear_check(bot,JAIL))
    {
        game->isInJail = 1;
        discard_gear(game,bot,JAIL);
        return;
    }
    
    pile_remain_manage(game);

    get_card(draw(game),bot);
    get_card(draw(game),bot);
    return;
}

