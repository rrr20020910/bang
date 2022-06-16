#include "utils/cstd.h"
#include "card.h"
#include "player.h"
#include "list.h"
#include "game.h"
#include "print.h"

// play card       ||     
//=================\/===
char *card_name(int32_t card_ID)
{
    char *name[3] = {"Bang!","Miss","beer"};
    return name[(card_ID%100)-1];
} 

// DIE             ||     
//=================\/===
static void die(List *game,Player *bot){

    printf("%s Has been slain!!!\n",bot->user_name);
    
    if(bot == game->next)
    {
        printf("sheriff DIE\n");
        if((game->players_num == 2) && (bot->next->role_ID == RENEGADE))
        {
            game->win_role = RENEGADE;
            return;
        }
        game->win_role = OUTLAW;
        return;
    }
    
    //delete
    Player *delete = game->next;
    while(delete->next != bot)
    {
        delete = delete->next;
    }
    Player *temp = delete->next;
    delete->next = temp->next;
    free(temp);
    game->players_num--;
    
    //sheriff win check
    if((game->players_num == 1) || ((game->players_num == 2) && (bot->next->role_ID == DEPUTY_SHERIFF)))
    {
        game->win_role = SHERIFF;
    }
    return;
}

// damg & heal     ||     
//=================\/===
static void heal(Player *bot)
{
    if(bot->bullets < (bot->charater_ID/100))
    {
        bot->bullets++;
    }
    return;
}

void damg(List *game,Player *bot)
{
    bot->bullets--;
    if(bot->bullets <= 0)
    {
        Player *current = bot->next;
        for(int i=0;i<((game->players_num)-1);i++)
        {
            printf("%s is dying do you want ",bot->user_name);
            if(play_specify_card(game,current,103))
            {
                heal(bot);
            }
            if(bot->bullets > 0)
            {
                break;
            }
            current=current->next;
        }
    }
    if(bot->bullets <= 0)
    {
        die(game,bot);
    }
    return;
}

// brown card      ||     
//=================\/===
static int32_t Bang(List *game,Player *bot)
{
    if(game->bang_play != 0)
    {
        printf("you already play bang\n");
        return 1;
    }
    
    if(game->bang_play != 0)
    {
        printf("you already play bang\n");
        return 1;
    }
    
    int32_t Miss_flag = 0;
    
    printf("Bang! select a target\n");
    Player *target = select_range_player(game,bot,1);
    if(target == NULL)
    {
        printf("It's too far\n");
        return 1;
    }
    
    if(gear_check(target,BARREL))
    {
        printf("Miss! use BARREL\n");
        Miss_flag++;
    }
    
    if(play_specify_card(game,target,102))
    {
        Miss_flag++;
    }
    
    if(!Miss_flag)
    {
        damg(game,target);
    }
    
    game->bang_play++;
    return 0;
}

static int32_t Wells_Fargo(List *game,Player *bot)
{
    get_card(draw(game),bot);
    get_card(draw(game),bot);
    get_card(draw(game),bot);
    return 0;
}

static int32_t Stagecoach(List *game,Player *bot)
{
    get_card(draw(game),bot);
    get_card(draw(game),bot);
    return 0;
}

static int32_t Salon(List *game,Player *bot)
{
    Player *current = bot;
    do
    {
        heal(current);
        current=current->next;
    }while((current) != bot);
    return 0;
}

static int32_t Miss(List *game,Player *bot)
{
    printf("you can't player miss\n");
    return 1;
}

static int32_t Beer(List *game,Player *bot)
{
    if(game->players_num != 2)
    {
        heal(bot);
    }
    return 0;
}

static int32_t Panic(List *game,Player *bot)
{
    Player *target = select_other_player(game,bot);
    get_card(discard(target),bot);
    return 0;
}

static int32_t General_Store(List *game,Player *bot)
{
    Card *store[game->players_num];
    for(int i=0;i<(game->players_num);i++)
    {
        store[i] = draw(game);
    }
    Player *current = bot;
    for(int i=0;i<(game->players_num)-1;i++)
    {
        //list card
        for(int j=0;j<(game->players_num)-i;j++)
        {
            printf("%d) ",j+1);
            print_card(*store[j]);
        }
        printf("please select a card(General_Store): ");
        int32_t sel = 0;
        scanf("%d",&sel);
        sel--;
        get_card(store[sel],current);
        
        for(int j=sel;j<(game->players_num)-i;j++)
        {
            store[j] = store[j+1];
        }
        current=current->next;
    }
    get_card(store[0],current);
    return 0;
}

static int32_t Duel(List *game,Player *bot)
{
    Player *target = select_other_player(game,bot);
    Player *current = target;
    
    while(1)
    {
        if(!play_specify_card(game,current,101))
        {
            damg(game,current);
            break;
        }
        if(current == bot)
        {
            current=target;
        }
        else
        {
            current=bot;
        }
    }
    return 0;
}

static int32_t Gatling(List *game,Player *bot)
{
    Player *current = bot;
    current = current->next;
    
    while(current != bot)
    {
        if(!play_specify_card(game,current,102))
        {
            damg(game,current);
        }
        current=current->next;
    }
    return 0;
}

static int32_t Indians(List *game,Player *bot)
{
    Player *current = bot;
    current = current->next;
    
    while(current != bot)
    {
        if(!play_specify_card(game,current,101))
        {
            damg(game,current);
        }
        current=current->next;
    }
    return 0;
}

/*
static int32_t Cat_Balou(list *game,player *bot)
{

1) random handcard
2) gun if thy have one
x)...list target gear

need card_name function

char* (int32_t card_ID){

}

    player *target = select_player(game);
    int32_t count = 0;
    
    int32_t card_flag = 0;
    int32_t gun_flag = 0;
    int32_t gear_flag = 0;
    
    if(bot->cards_num)
    {
        count++;
        card_flag++;
        printf("%d) discard %s random hand card\n",count,bot->user_name);
    }
    if(bot->gun)
    {
        count++;
        gun_flag++;
        printf("%d) discard %s gun\n",count,bot->user_name);
    }
    for(int i=0;i<bot->gear_num;i++)
    {
        count++;
        gear_flag++;
        printf("%d) discard %s %s\n",count,bot->user_name,card_name(bot->gear[i]));
    }
    printf("Please enter selection : ");
    int32_t sel = 0;
    scanf("%d",&sel);
    
    if(sel == card_flag)
    {
    
    }
    if(sel == gun_flag)
    {
    
    }
    discard_random(target);
}
*/

// blue card       ||     
//=================\/===
static int32_t Jail(List *game,Player *bot)
{
    printf("Jail select a target\n");
    Player *target = select_other_player(game,bot);
    
    if(gear_check(target,game->current_card->card_ID))
    {
        return 1;
    }
    
    Card *equip = game->current_card;
    target->gear[target->gear_num] = *equip;
    target->gear_num ++;
    
    return 0;
}

int32_t Dynamite(List *game,Player *bot)
{
    bot->gear[bot->gear_num] = *game->current_card;
    bot->gear_num ++;
    
    return 0;
}

static int32_t Barrel(List *game,Player *bot)
{
    if(gear_check(bot,game->current_card->card_ID))
    {
        printf("you already have %s",game->current_card->name);
        return 1;
    }
    bot->gear[bot->gear_num] = *game->current_card;
    bot->gear_num ++;
    
    return 0;
}

// pile            ||     
//=================\/=== 
void build_pile(List *game)
{    
    Card pile[CARD_TPYE] = {
        {"Bang!"         ,101,&Bang},
        {"Miss"          ,102,&Miss},
        {"Beer"          ,103,&Beer},
        {"Cat_Balou"     ,104,&Duel},
        {"Panic"         ,105,&Panic},
        {"Duel"          ,106,&Duel},
        {"General_Store" ,107,&General_Store},
        {"Indians"       ,108,&Indians},
        {"Gatling"       ,109,&Gatling},
        {"Salon"         ,110,&Salon},
        {"Wells_Fargo"   ,111,&Wells_Fargo},
        {"Stagecoach"    ,112,&Stagecoach},
        {"Jail"          ,213,&Jail},
        {"Dynamite"      ,214,&Dynamite},
        {"Barrel"        ,215,&Barrel}
    };
                    
    int32_t every_card_num[CARD_TPYE] = {25,12,6,4,4,3,2,2,1,1,1,2,3,1,3};
    
    int32_t count=0;
    for(int i=0;i<CARD_TPYE;i++)
    {
        for(int j=0;j<every_card_num[i];j++)
        {
            game->pile[count] = pile[i];
            count++;
        }
    }
    
    srand(time(NULL));
    
    for(int i=0;i<CARD_NUM;i++)
    {
        game->pile[i].suit = rand()%4+1;
        game->pile[i].face = rand()%13+1;
    }
    
    for (int i=0;i<CARD_NUM;i++)
    {
        int32_t rand_pos = rand()%CARD_NUM;
        Card temp = game->pile[i];
        game->pile[i] = game->pile[rand_pos];
        game->pile[rand_pos] = temp;
    }
    game->pos = 0;
    return;
}
