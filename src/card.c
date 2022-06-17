#include "utils/cstd.h"
#include "card.h"
#include "player.h"
#include "list.h"
#include "print.h"

// play card       ||     
//=================\/===
static char *card_name(int32_t card_ID)
{
    char *name[3] = {"Bang!","Miss","beer"};
    return name[(card_ID%100)-1];
} 

static int32_t play_specify_card(List *game,Player *bot,int32_t card_id)
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
    
    Card *current = &(bot->hand_card[sel]);
    game->discard_pile[game->discard_pos] = *current;
    game->discard_pos++;
    
    for(int i=sel;i<(bot->cards_num);i++)
    {
        bot->hand_card[i] = bot->hand_card[i+1];
    }
    bot->cards_num--;
    memset(&(bot->hand_card[bot->cards_num]),0,sizeof(Card));
    return 1;
}

// discard random  ||     
//=================\/===
static Card *discard_random(List *game,Player *bot)
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

int32_t gear_check(Player *bot,int32_t gear_ID)
{
    for(int i=0;i < bot->gear_num;i++)
    {
        if(bot->gear[i].card_ID == gear_ID)
        {
            return i+1;
        }
    }
    return 0;
}

// DIE             ||     
//=================\/===
static void die(List *game,Player *bot)
{
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

static int32_t get_player_range(Player *bot)
{
    int32_t range = 1;
    if(gear_check(bot,VOLANIC))
    {
        return range;
    }
    if(gear_check(bot,SCHOFIELD))
    {
        range = 2;
    }
    if(gear_check(bot,REMINGTON))
    {
        range = 3;
    }
    if(gear_check(bot,CARABINE))
    {
        range = 4;
    }
    if(gear_check(bot,WINCHESTER))
    {
        range = 5;
    }
    
    if(gear_check(bot,MUSTANG))
    {
        range++;
    }
    return range;
}

// brown Card      ||     
//=================\/===
static int32_t Bang(List *game,Player *bot)
{
    if(game->bang_play != 0)
    {
        printf("you already play bang\n");
        return 1;
    }
    
    int32_t Miss_flag = 0;

    printf("Bang! select a target\n");
    Player *target = select_range_player(game,bot,get_player_range(bot));
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
    
    if(!gear_check(bot,VOLANIC))
    {
        game->bang_play++;
    }
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
    printf("you can't Player miss\n");
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
    Player *target = select_range_player(game,bot,SIDE);
    if(target == NULL)
    {
        printf("It's too far\n");
        return 1;
    }
    get_card(discard_random(game,target),bot);
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
        //List card
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

static int32_t Cat_Balou(List *game,Player *bot)
{
    Player *target = select_other_player(game,bot);
       
    printf("1) disCard random hand card\n");
    for(int i=0;i < target->gear_num;i++)
    {
        printf("%d) disCard %s\n",i+2,target->gear[i].name);
    }
    printf("Please enter selection :");
    int32_t sel = 0;
    scanf("%d",&sel);
    
    if(sel == 1)
    {
        discard_random(game,target);
        return 0;
    }
    sel-=2;
    if(sel <= bot->gear_num)
    {
        discard_gear(game,target,target->gear[sel].card_ID);
        return 0;
    }
    return 1;
}

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
    target->gear_num++;
    return 0;
}

int32_t Dynamite(List *game,Player *bot)
{
    bot->gear[bot->gear_num] = *game->current_card;
    bot->gear_num++;
    return 0;
}

static int32_t Barrel(List *game,Player *bot)
{    
    if(gear_check(bot,BARREL))
    {
        printf("you already have BARREL");
        return 1;
    }
    bot->gear[bot->gear_num] = *game->current_card;
    bot->gear_num ++;
    
    return 0;
}

static int32_t Gun(List *game,Player *bot)
{
    for(int i=SCHOFIELD ;i <= WINCHESTER;i++)
    {
        if(gear_check(bot,i))
        {
            printf("you already have BARREL");
            return 1;
        }
    }
    bot->gear[bot->gear_num] = *game->current_card;
    bot->gear_num ++;
    return 0;
}

static int32_t Horse(List *game,Player *bot)
{
    if(gear_check(bot,MUSTANG))
    {
        printf("you already have BARREL");
        return 1;
    }
    if(gear_check(bot,APPALOOSA))
    {
        printf("you already have BARREL");
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
        {"Cat_Balou"     ,104,&Cat_Balou},
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
        {"Barrel"        ,215,&Barrel},
        {"Mustang"       ,216,&Horse},
        {"Appaloosa"     ,217,&Horse},
        {"Schofield"     ,218,&Gun},
        {"Volanic"       ,219,&Gun},
        {"Remington"     ,220,&Gun},
        {"Rev.carabine"  ,221,&Gun},
        {"Winchester"    ,222,&Gun}
    };
                    
    int32_t every_card_num[CARD_TPYE] = {25,12,6,4,4,3,2,2,1,1,1,2,2,1,3,2,1,3,2,1,1,1};
    
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
    
    for(int i=0;i<CARD_NUM;i++)
    {
        int32_t rand_pos = rand()%CARD_NUM;
        Card temp = game->pile[i];
        game->pile[i] = game->pile[rand_pos];
        game->pile[rand_pos] = temp;
    }
    game->pile_pos = 0;
    game->discard_pos = 0;
    return;
}
