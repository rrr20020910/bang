#include "utils/cstd.h"
#include "card.h"
#include "player.h"
#include "list.h"
#include "print.h"

// play card       ||     
//=================\/===
char *card_name(int32_t card_ID)
{
    char *name[CARD_TPYE] = {
        BANG_NAME,
        MISS_NAME,
        BEER_NAME,
        CAT_BLAUE_NAME,
        PANIC_NAME,
        DUEL_NAME,
        GENERAL_STORE_NAME,
        INDIANS_NAME,
        GATLING_NAME,
        SALON_NAME,
        WELLS_FARGO_NAME,
        STAGECOACH_NAME,
        JAIL_NAME,
        DYNAMITE_NAME,
        BARREL_NAME,
        MUSTANG_NAME,
        APPALOOSA_NAME,
        SCHOFIELD_NAME,
        VOLANIC_NAME,
        REMINGTON_NAME,
        CARABINE_NAME,
        WINCHESTER_NAME
    };
    return name[(card_ID%100)-1];
} 

static int32_t play_specify_card(List *game,Player *bot,int32_t card_id)
{
    INFO_MSG_PRINT("You need to play %s!!",card_name(card_id));
    
    int32_t sel = get_temp_player_play(game,bot);
    
    if(sel == 9)
    {
        return 0;
    }
    
    //======Calamity_Janet=====
    if(bot->charater_ID == Calamity_Janet)
    {
        if(card_id == BEER)
        {
            return 0;
        }
        SYS_BAR_PRINT("%s use Calamity_Janet skill",bot->user_name);
    }
    //===============================
    else if(bot->hand_card[sel].card_ID != card_id)
    {
        return 0;
    }
    SYS_BAR_PRINT("%s play %s as reaction",bot->user_name,card_name(card_id));
    Card *current = &(bot->hand_card[sel]);
    game->discard_pile[game->discard_pos] = *current;
    game->discard_pos++;
    
    for(int i=sel;i<(bot->cards_num);i++)
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
            SYS_BAR_PRINT("%s use Suzy_Lafayette skill draw a card",bot->user_name);
            get_card(draw(game),bot);
        }
    }
    //=========================
    
    return 1;
}

// discard random  ||     
//=================\/===
Card *discard_random(List *game,Player *bot)
{
    srand(time(NULL));
    int32_t random_card = rand()%(bot->cards_num); 
    
    Card *discard_card = &bot->hand_card[random_card];
    
    SYS_BAR_PRINT("%s discard %s",bot->user_name,card_name(bot->hand_card[random_card].card_ID));
    
    for(int i=random_card;i<(bot->cards_num);i++)
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
            SYS_BAR_PRINT("%s use Suzy_Lafayette skill draw a card",bot->user_name);
            get_card(draw(game),bot);
        }
    }
    //=========================
    
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
    SYS_BAR_PRINT("\033[1;31m%s has been slain!!\033[0m",bot->user_name);
    if(bot == game->next)
    {
        SYS_BAR_PRINT("\033[1;31mSheriff DIE!!\033[0m");
        if((game->players_num == 2) && (bot->next->role_ID == RENEGADE))
        {
            game->win_role = RENEGADE;
            return;
        }
        game->win_role = OUTLAW;
        return;
    }

    if(bot->role_ID == OUTLAW)
    {
        SYS_BAR_PRINT("%s get three cards because outlaw has been slained ",game->current_player->user_name);
        get_card(draw(game),game->current_player); 
        get_card(draw(game),game->current_player); 
        get_card(draw(game),game->current_player); 
    }
    
    //======Vulture_Sam=====
    Player *current = bot;
    current = current->next;
    
    while(current != bot)
    {
        if(current->charater_ID == Vulture_Sam)
        {
            break;
        }
        current=current->next;
    }
    
    if(current->charater_ID == Vulture_Sam)
    {
        for(int i=0;i<bot->cards_num;i++){
            SYS_BAR_PRINT("%s use Vulture_Sam get card from %s!!",current->user_name,bot->user_name);
            get_card(&bot->hand_card[i],current);
        }
        for(int i=0;i<bot->gear_num;i++){
            SYS_BAR_PRINT("%s use Vulture_Sam get card from %s!!",current->user_name,bot->user_name);
            get_card(&bot->gear[i],current);
        }
    }
    //=========================
    
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
void heal(Player *bot)
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
    SYS_BAR_PRINT("%s take one damg",bot->user_name);
    //======Bart_Cassidy======
    if(bot->charater_ID == Bart_Cassidy){
        SYS_BAR_PRINT("%s use Bart_Cassidy get a card",bot->user_name);
        get_card(draw(game),bot);
    }
    //========================
    
    //=======El_Gringo=======
    if(bot->charater_ID == El_Gringo){
        SYS_BAR_PRINT("%s use El_Gringo get card from %s",bot->user_name,game->current_player->user_name);
        get_card(discard_random(game,game->current_player),bot);
    }
    //========================

    if(bot->bullets <= 0)
    {
        INFO_MSG_PRINT("you are dying, you can play beer to your live.");
        if(play_specify_card(game,bot,BEER))
        {
            heal(bot);
        }
        if(bot->bullets <= 0){
            Player *current = bot->next;
            for(int i=0;i<(game->players_num-1);i++)
            {
                INFO_MSG_PRINT("%s are dying, you can play beer to save his live.",bot->user_name);
                if(play_specify_card(game,current,BEER))
                {
                    heal(bot);
                    if(bot->bullets > 0)
                    {
                        break;
                    }
                }
                current=current->next;
            }
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
    
    //======Rose_Doolan======
    if(bot->charater_ID == Rose_Doolan){
        range++;
    }
    //========================
    
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
        WARNING_MSG_PRINT("You can't not play bang anymore!!");
        return 1;
    }
    
    int32_t Miss_flag = 0;

    INFO_MSG_PRINT("Bang! Please select a target: ");
    Player *target = select_range_player(game,bot,get_player_range(bot));
    if(target == NULL)
    {
        WARNING_MSG_PRINT("No, It's too far!! Your distance is not enough to Bang target!!");
        usleep(2000000);
        return 1;
    }
    
    SYS_BAR_PRINT("%s play Bang! shoot %s",bot->user_name,target->user_name);
    
    if(gear_check(target,BARREL))
    {
        //======Lucky_Duke=====
        if(bot->charater_ID == Lucky_Duke){
            SYS_BAR_PRINT("%s use Lucky_Duke double check!",bot->user_name);
            Miss_flag++;
        }
        //=======================
        SYS_BAR_PRINT("%s Miss! because of the BARREL!! It's too lucky!!",target->user_name);
        Miss_flag++;
    }
    
    //=======Jourdonnais=======
    if(bot->charater_ID == Jourdonnais){
        SYS_BAR_PRINT("%s Miss! because of the BARREL!! It's too lucky!!",target->user_name);
        Miss_flag++;
    }
    //========================
    
    if(play_specify_card(game,target,102))
    {
        Miss_flag++;
    }
    //======Slab_the_Killer=====
    if(bot->charater_ID == Slab_the_Killer){
        SYS_BAR_PRINT("%s is Slab_the_Killer target have to play double miss",bot->user_name);
        if(Miss_flag == 1){
            Miss_flag--;
            if(play_specify_card(game,target,102))
            {
                Miss_flag++;
            }
        }
    }
    //========================
    if(!Miss_flag)
    {
        damg(game,target);
    }
    
     game->bang_play++;
    
    if(gear_check(bot,VOLANIC))
    {
        SYS_BAR_PRINT("%s has VOLANIC can still play Bang",bot->user_name);
        game->bang_play = 0;
    }
    //======Willy_the_Kid=====
    else if(bot->charater_ID == Willy_the_Kid){
        SYS_BAR_PRINT("%s is Willy_the_Kid can still play Bang",bot->user_name);
        game->bang_play = 0;
    }
    //========================
    return 0;
}

static int32_t Wells_Fargo(List *game,Player *bot)
{
    SYS_BAR_PRINT("%s play Wells_Fargo draw three cards",bot->user_name);
    get_card(draw(game),bot);
    get_card(draw(game),bot);
    get_card(draw(game),bot);
    return 0;
}

static int32_t Stagecoach(List *game,Player *bot)
{
    SYS_BAR_PRINT("%s play Stagecoach draw two cards",bot->user_name);
    get_card(draw(game),bot);
    get_card(draw(game),bot);
    return 0;
}

static int32_t Salon(List *game,Player *bot)
{
    SYS_BAR_PRINT("%s play Saloon heal every one bullet",bot->user_name);
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
    //======Calamity_Janet=====
    if(bot->charater_ID == Calamity_Janet){
        SYS_BAR_PRINT("%s is Calamity_Janet play miss as Bang!",bot->user_name);
        if(Bang(game,bot)){
            return 1;
        }
        return 0;
    }
    //========================
    WARNING_MSG_PRINT("You can't play miss\n");
    return 1;
}

static int32_t Beer(List *game,Player *bot)
{
    if(bot->bullets == (bot->charater_ID/100)){
        WARNING_MSG_PRINT("You don't need play beer now\n");
        return 1;
    }
    if(game->players_num != 2)
    {
        heal(bot);
    }
    SYS_BAR_PRINT("%s play beer bullets+=1 ",bot->user_name);
    return 0;
}

static int32_t Panic(List *game,Player *bot)
{
    Player *target = select_range_player(game,bot,SIDE);
    if(target == NULL)
    {
        WARNING_MSG_PRINT("It's too far\n");
        return 1;
    }
    get_card(discard_random(game,target),bot);
    
    SYS_BAR_PRINT("%s play panic get a card from %s",bot->user_name,target->user_name);
    return 0;
}

static int32_t General_Store(List *game,Player *bot)
{
    SYS_BAR_PRINT("%s play General_Store",bot->user_name);
    Card *store[game->players_num];
    for(int i=0;i<(game->players_num);i++)
    {
        store[i] = draw(game);
    }
    Player *current = bot;
    for(int i=0;i<(game->players_num)-1;i++)
    {
        print_store(1,1,112,14);
        for(int i=0;i<6;i++)
        {
            if(i < game->players_num-i)
            {
                print_card_visual(5+i*17,2,*store[i]);
                gotoxy(7+i*17,13);
                printf("[_%d_]",i+1);
            }
            else
            {
                Card Null_card;
                memset(&Null_card,0,sizeof(Card));
                print_card_visual(5+i*17,2,Null_card);
            }
        }
        gotoxy(2,16);
        printf("please select a card(General_Store) %s: ",current->user_name);
        int32_t sel = 0;
        CHECK_UNTIL(sel>=1&&sel<=game->players_num-i,sel,"please select a valid card");
        sel--;
        get_card(store[sel],current);
        
        SYS_BAR_PRINT("%s get a card from General_Store",current->user_name);
        
        for(int j=sel;j<(game->players_num)-i;j++)
        {
            store[j] = store[j+1];
        }
        current=current->next;
    }
    SYS_BAR_PRINT("%s get a card from General_Store",current->user_name);
    get_card(store[0],current);
    return 0;
}

static int32_t Duel(List *game,Player *bot)
{
    Player *target = select_other_player(game,bot);
    if(target == NULL){
        return 1;
    }
    
    SYS_BAR_PRINT("%s duel %s",bot->user_name,target->user_name);
    
    if(target == NULL){
        return 1;
    }
    
    
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
    SYS_BAR_PRINT("%s shoot every one play miss to avoid damg",bot->user_name);
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
    SYS_BAR_PRINT("%s play Indians play bang to avoid damg",bot->user_name);
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
    

    if(target == NULL){
        return 1;
    }
    printf("1) disCard random hand card\n");
    for(int i=0;i < target->gear_num;i++)
    {
        printf("%d) disCard %s\n",i+2,target->gear[i].name);
    }
    printf("Please enter selection :");
    int32_t sel = 0;
    CHECK_UNTIL(sel>=1&&sel<=7,sel,"please select a valid player");
    
    if(sel == 1)
    {
        SYS_BAR_PRINT("%s play Cat_Balou %s discard hand card",bot->user_name,target->user_name);
        discard_random(game,target);
        return 0;
    }
    sel-=2;
    if(sel <= bot->gear_num)
    {
        SYS_BAR_PRINT("%s play Cat_Balou %s discard %s",bot->user_name,target->user_name,card_name(target->gear[sel].card_ID));
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
    
    if(target->role_ID == SHERIFF)
    {
        WARNING_MSG_PRINT("You can't lock SHERIFF in Jail\n");
        return 1;
    }
    
    if(target == NULL){
        WARNING_MSG_PRINT("choose right target\n");
        return 1;
    }
    
    if(gear_check(target,game->current_card->card_ID))
    {
        WARNING_MSG_PRINT("%s already in Jail\n",target->user_name);
        return 1;
    }
    
    SYS_BAR_PRINT("%s play Jail lock %s in Jail",bot->user_name,target->user_name);
    Card *equip = game->current_card;
    target->gear[target->gear_num] = *equip;
    target->gear_num++;
    return 0;
}

int32_t Dynamite(List *game,Player *bot)
{
    SYS_BAR_PRINT("%s equip Dynamite",bot->user_name);
    bot->gear[bot->gear_num] = *game->current_card;
    bot->gear_num++;
    return 0;
}

static int32_t Barrel(List *game,Player *bot)
{    
    if(gear_check(bot,BARREL))
    {
        if(game->gear_change == BARREL){
            discard_gear(game,bot,BARREL);
            bot->gear[bot->gear_num] = *game->current_card;
            bot->gear_num ++;
            return 0;
        }
        WARNING_MSG_PRINT("you already have Barrel play again to equip\n");
        game->gear_change = BARREL;
        return 1;
    }
    SYS_BAR_PRINT("%s equip Barrel",bot->user_name);
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
            if(game->gear_change == i)
            {
                discard_gear(game,bot,i);
                bot->gear[bot->gear_num] = *game->current_card;
                bot->gear_num ++;
                return 0;
            }
            WARNING_MSG_PRINT("you already have gun play again to equip\n");
            game->gear_change = i;
            return 1;
        }
        
    }

    bot->gear[bot->gear_num] = *game->current_card;
    SYS_BAR_PRINT("%s equip %s",bot->user_name,card_name(bot->gear[bot->gear_num].card_ID));
    bot->gear_num ++;
    return 0;
}

static int32_t Horse(List *game,Player *bot)
{
    for(int i=MUSTANG ;i <= APPALOOSA;i++)
    {
        if(gear_check(bot,i))
        {
            if(game->gear_change == i)
            {
                discard_gear(game,bot,i);
                bot->gear[bot->gear_num] = *game->current_card;
                bot->gear_num ++;
                return 0;
            }
            WARNING_MSG_PRINT("you already have horse play again to equip\n");
            game->gear_change = i;
            return 1;
        }
        
    }
    bot->gear[bot->gear_num] = *game->current_card;
    SYS_BAR_PRINT("%s equip %s",bot->user_name,card_name(bot->gear[bot->gear_num].card_ID));
    bot->gear_num ++;
    return 0;
}

// pile            ||     
//=================\/=== 
void build_pile(List *game)
{    
    Card pile[CARD_TPYE] = {
        {BANG_NAME          ,BANG           ,&Bang},
        {MISS_NAME          ,MISS           ,&Miss},
        {BEER_NAME          ,BEER           ,&Beer},
        {CAT_BLAUE_NAME     ,CAT_BLAUE      ,&Cat_Balou},
        {PANIC_NAME         ,PANIC          ,&Panic},
        {DUEL_NAME          ,DUEL           ,&Duel},
        {GENERAL_STORE_NAME ,GENERAL_STORE  ,&General_Store},
        {INDIANS_NAME       ,INDIANS        ,&Indians},
        {GATLING_NAME       ,GATLING        ,&Gatling},
        {SALON_NAME         ,SALON          ,&Salon},
        {WELLS_FARGO_NAME   ,WELLS_FARGO    ,&Wells_Fargo},
        {STAGECOACH_NAME    ,STAGECOACH     ,&Stagecoach},
        {JAIL_NAME          ,JAIL           ,&Jail},
        {DYNAMITE_NAME      ,DYNAMITE       ,&Dynamite},
        {BARREL_NAME        ,BARREL         ,&Barrel},
        {MUSTANG_NAME       ,MUSTANG        ,&Horse},
        {APPALOOSA_NAME     ,APPALOOSA      ,&Horse},
        {SCHOFIELD_NAME     ,SCHOFIELD      ,&Gun},
        {VOLANIC_NAME       ,VOLANIC        ,&Gun},
        {REMINGTON_NAME     ,RENEGADE       ,&Gun},
        {CARABINE_NAME      ,CARABINE       ,&Gun},
        {WINCHESTER_NAME    ,WINCHESTER     ,&Gun}
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

void pile_remain_manage(List *game)
{
    if(game->pile[game->pile_pos].card_ID == NULL_CARD) // check card pile empty
    {
        memcpy(game->pile,game->discard_pile,sizeof(Card)*PILE_NUM);
        memset(&(game->discard_pile),0,sizeof(Card)*PILE_NUM);
        
        game->discard_pos = 0;
        game->pile_pos = 0;
        int32_t current_pile_num = 0;
        for(int i=0;game->pile[i].card_ID != NULL_CARD;i++)
        {   
            current_pile_num = i + 1;
        }
        
        srand(time(NULL));
        
        for(int i=0;i<current_pile_num;i++)
        {
            int32_t rand_pos = rand()%(current_pile_num);
            Card temp = game->pile[i];
            game->pile[i] = game->pile[rand_pos];
            game->pile[rand_pos] = temp;
        }
    }
}
