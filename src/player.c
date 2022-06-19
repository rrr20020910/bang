#include "utils/cstd.h"
#include "player.h"
#include "card.h"
#include "list.h"
#include "print.h"

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
    int32_t distance = compare(right,left);
    
    if(gear_check(bot1,MUSTANG))
    {
        distance++;
    }
    
    //======Paul_Regret=======
    if(bot1->charater_ID == Paul_Regret)
    {
        distance++;
    }
    //========================

    return distance;
}

// select          ||     
//=================\/===
Player *select_other_player(List *game,Player *bot)
{
    Player *current = bot;
    
    printf("please select a player:");
    int32_t sel = 0;
    scanf("%d",&sel);
    
    if(sel > game->players_num || sel <= 0){
        return NULL;
    }
    
    for(int i=0;i<sel;i++)
    {
        current = current->next;
    }
    return current;
}

Player *select_range_player(List *game,Player *bot,int32_t range)
{
    Player *select = select_other_player(game,bot);
    
    if(select == NULL)
    {
        return NULL;
    }
    
    if(range == SIDE)
    {
        if((select->next == bot) || (bot->next == select))
        {
            return select;
        }
        else
        {
            return NULL;
        }
    }
    if(get_distance(select,bot) > range)
    {
        return NULL;
    }
    
    return select;
}

void get_card(Card *new_card,Player *bot)
{
    bot->hand_card[bot->cards_num] = *new_card;
    bot->cards_num++;
    return;
}

// draw & get      ||     
//=================\/===
Card *draw(List *game)
{
    pile_remain_manage(game);
    int32_t ori_pos = game->pile_pos;
    game->pile_pos++;
    return &game->pile[ori_pos];
}

Card *discard_gear(List *game,Player *bot,int32_t gear_ID)
{
    int32_t pos = gear_check(bot,gear_ID)-1;
    Card *discard_card = &bot->gear[pos];
    
    game->discard_pile[game->discard_pos] = bot->gear[pos];
    game->discard_pos++;
    
    for(int i=pos;i<(bot->gear_num);i++)
    {
        bot->gear[i] = bot->gear[i+1];
    }
    bot->gear_num--;
    memset(&(bot->gear[bot->gear_num]),0,sizeof(Card));
    
    return discard_card;
}

// discard         ||     
//=================\/===

int32_t get_temp_player_play(List *game,Player *bot)
{
    while(1)
    {
        print_board(game,bot);
        int32_t sel=0;
        printf("Please select a card:");
        CHECK_UNTIL(sel>=1&&sel<=7,sel,"Please input valid move!!");
        
        //0,7 page control
        if(sel == 0)
        {
            if(game->card_page == 0)
            {
                continue;
            }
            game->card_page --;
            continue;
        }
        if(sel == 7)
        {
            game->card_page ++;
            if(game->card_page > 3)
            {
                game->card_page = 0;
            }
        }
        if(sel < 7 && sel > 0)
        {
            if((sel+(game->card_page)*6-1) < bot->cards_num)
            {
                return (sel+(game->card_page)*6-1);
            }
        }
    }
    return 0;
}


Card *select_discard(List *game,Player *bot)
{
    
    int32_t discard_pos = get_temp_player_play(game,bot);
    
    Card *discard_card = &bot->hand_card[discard_pos];
    
    Card *current = &(bot->hand_card[discard_pos]);
    game->discard_pile[game->discard_pos] = *current;
    game->discard_pos++;
    
    for(int i=discard_pos;i<(bot->cards_num);i++)
    {
        bot->hand_card[i] = bot->hand_card[i+1];
    }
    
    bot->cards_num--;
    memset(&(bot->hand_card[bot->cards_num]),0,sizeof(Card));
    
    //======Suzy_Lafayette=====
    if(bot->charater_ID == Suzy_Lafayette)
    {
        if(bot->cards_num == 0)
        {
            get_card(draw(game),bot);
        }
    }
    //=========================
    
    return discard_card;
}




