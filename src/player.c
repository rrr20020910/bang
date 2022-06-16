#include "utils/cstd.h"
#include "player.h"
#include "card.h"
#include "game.h"
#include "print.h"

void get_card(Card *new_card,Player *bot)
{
    bot->hand_card[bot->cards_num] = *new_card;
    bot->cards_num++;
    new_card->is_available = 0;
    return;
}

// available move  ||     
//=================\/===
static int32_t play_card(List *game,Player *bot,int32_t sel)
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
    
    for(int i=sel;i<(bot->cards_num);i++)
    {
        bot->hand_card[i] = bot->hand_card[i+1];
    }
    bot->cards_num--;
    
    memset(&(bot->hand_card[bot->cards_num]),0,sizeof(Card));
    return 0;
}

int32_t available(List *game,Player *bot)
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
        /*
        if(game->card_page*6 < (bot->cards_num))
        {
            printf("you can't do that");
            return 0;
        }*/
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

// select          ||     
//=================\/===
Player *select_other_player(List *game,Player *bot)
{
    Player *current = bot;
    
    printf("please select a player:");
    int32_t sel = 0;
    scanf("%d",&sel);
    
    for(int i=0;i<sel;i++)
    {
        current = current->next;
    }
    return current;
}

#define SIDE 0
Player *select_range_player(List *game,Player *bot,int32_t range)
{    
    Player *select = select_other_player(game,bot);
    
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

int32_t play_specify_card(List *game,Player *bot,int32_t card_id)
{    
    printf("play %s\n",card_name(card_id));
    print_player_hand(bot);
    printf("%d) I don't want to play\n",bot->cards_num);
    printf("please select a card:");
    int32_t sel=0;
    scanf("%d",&sel);
    sel--;
    if(bot->hand_card[sel].card_ID != card_id)
    {
        return 0;
    }
    
    for(int i=sel;i<(bot->cards_num);i++)
    {
        bot->hand_card[i] = bot->hand_card[i+1];
    }
    bot->cards_num--;
    memset(&(bot->hand_card[bot->cards_num]),0,sizeof(Card));
    return 1;
}

// discard         ||     
//=================\/===
Card *discard(Player *bot)
{
    srand(time(NULL));
    int32_t random_card = rand()%(bot->cards_num); 
    
    Card *discard_card = &bot->hand_card[random_card];
    
    for(int i=random_card;i<(bot->cards_num);i++)
    {
        bot->hand_card[i] = bot->hand_card[i+1];
    }
    
    bot->cards_num--;
    memset(&(bot->hand_card[bot->cards_num]),0,sizeof(Card));
    return discard_card;
}

// gear check      ||     
//=================\/===
int32_t gear_check(Player *bot,int32_t gear_ID)
{
    for(int i=0;i<bot->gear_num;i++)
    {
        if(bot->gear[bot->gear_num].card_ID == gear_ID)
        {
            return i+1;
        }
    }
    return 0;
}

// discard gear    ||     
//=================\/===
Card *discard_gear(Player *bot,int32_t gear_ID)
{    
    int32_t pos = gear_check(bot,gear_ID)-1;
    Card *discard_card = &bot->gear[pos];
    
    for(int i=pos;i<(bot->gear_num);i++)
    {
        bot->gear[i] = bot->gear[i+1];
    }
    bot->gear_num--;
    memset(&(bot->gear[bot->gear_num]),0,sizeof(Card));
    
    return discard_card;
}
