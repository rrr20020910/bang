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
    
    if(bot->hand_card[sel].card_ID == 0){
    
        SYS_BAR_PRINT("Our apologize for that thing happen,you can draw another");
        get_card(draw(game),bot);
        return 0;
    }
    
    
    if(bot->hand_card[sel].ability(game,bot))
    {
        printf("fail play\n");
        return 0;
    }

    if((bot->hand_card[sel].card_ID)/100 == 1)
    {
        game->discard_pile[game->discard_pos] = *game->current_card;
        game->discard_pos++;
    }

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
            get_card(draw(game),bot);
        }
    }
    //=========================
    
    return 0;
}

int32_t get_player_move(List *game,Player *bot)
{
    //get select
    printf("Please select a move:");
    int32_t sel=0;
    CHECK_UNTIL(sel >= 1 && sel <= 9,sel,"You have to choose an action");
    
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
            printf("you can't do that!");
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
    
    //======Sid_Ketchum======
    if(bot->charater_ID == Sid_Ketchum)
    {
        if(sel == 8)
        {
            SYS_BAR_PRINT("%s use Sid_Ketchum's skill to restore one bullet.",bot->user_name);
            select_discard(game,bot);
            select_discard(game,bot);
            heal(bot);
        }
    }
    //========================
    
    //9 leave
    if(sel == 9)
    {
        return 1;
    }
    
    return 0;
}

void set_role(List *game)
{
    int32_t role_list[8] = {
        SHERIFF,
        RENEGADE,
        OUTLAW,
        OUTLAW,
        DEPUTY_SHERIFF,
        OUTLAW,
        DEPUTY_SHERIFF,
        RENEGADE
    };
    srand(time(NULL));
    
    Player *current = game->next;
    
    for(int i=0;i<game->players_num;i++)
    {
        int32_t rand_pos = rand()%game->players_num;
        int32_t temp = role_list[i];
        role_list[i] = role_list[rand_pos];
        role_list[rand_pos] = temp;
    }
    for(int i=0;i<game->players_num;i++)
    {
        current->role_ID = role_list[i];
        current = current->next;
    }
}

void set_character(List *game){

    int32_t charater_list[CHARACTER_NUM] = {
        Bart_Cassidy,
        Black_Jack,
        Calamity_Janet,
        El_Gringo,
        Jesse_Jones,
        Jourdonnais,
        Kit_Carlson,
        Lucky_Duke,
        Paul_Regret,
        Pedro_Ramirez,
        Rose_Doolan,
        Sid_Ketchum,
        Slab_the_Killer,
        Suzy_Lafayette,
        Vulture_Sam,
        Willy_the_Kid
    };
    srand(time(NULL));
    for(int i=0;i<CHARACTER_NUM;i++)
    {
        int32_t rand_pos = rand()%CHARACTER_NUM;
        int32_t temp = charater_list[i];
        charater_list[i] = charater_list[rand_pos];
        charater_list[rand_pos] = temp;
    }
    
    Player *current = game->next;
    for(int i=0;i<game->players_num;i++)
    {
        current->charater_ID = charater_list[i];
        current->bullets = (current->charater_ID)/100;
        for(int i=0;i<(current->bullets);i++)
        {
            get_card(draw(game),current);
        }
        current = current->next;
    }
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
        int check = 0;
        //======Lucky_Duke=====
        if(bot->charater_ID == Lucky_Duke){
            SYS_BAR_PRINT("%s use Lucky_Duke's great luck!",bot->user_name);
            check++;
        }
        //========================
        
        if(check == 0){
            damg(game,bot);
        }

        Card *current = &(bot->gear[0]);
        game->current_card = current;
        
        Dynamite(game,bot->next);
        
        discard_gear(game,bot,DYNAMITE);
    }
    
    if(gear_check(bot,JAIL))
    {
        int check = 0;
        
        //======Lucky_Duke=====
        if(bot->charater_ID == Lucky_Duke){
            SYS_BAR_PRINT("%s use Lucky_Duke's great luck!'",bot->user_name);
            check++;
        }
        //========================
        
        if(check == 0){
            game->isInJail = 1;
        }
        discard_gear(game,bot,JAIL);
        return;
    }

    //======Pedro_Ramirez=====
    if(bot->charater_ID == Pedro_Ramirez){
        printf("You are Pedro_Ramirez, do you want to draw from discard pile?[_8_]");
        
        int32_t sel = 0;
        CHECK_UNTIL(sel==8,sel,"No next time!");
        if(sel == 8){
            get_card(&game->discard_pile[game->discard_pos-1],bot);
            memset(&game->discard_pile[game->discard_pos-1],0,sizeof(Card));
            game->discard_pos--;
            SYS_BAR_PRINT("%s use Pedro_Ramirez's skill to draw from discard pile.",bot->user_name);
            get_card(draw(game),bot);
            return;
        }
        else{
            get_card(draw(game),bot);
            get_card(draw(game),bot);
            return;
        }
        

    }
    //========================
    
    //======Jesse_Jones=====
    if(bot->charater_ID == Jesse_Jones){
        printf("You are Jesse_Jones, do you want to draw from other player?[_8_]");
        int32_t sel = 0;
        CHECK_UNTIL(sel==8,sel,"No next time!");
        if(sel == 8){
            Player *target = select_other_player(game,bot);
            get_card(discard_random(game,target),bot);
            get_card(draw(game),bot);
            return;
        }
        else{
            get_card(draw(game),bot);
            get_card(draw(game),bot);
            return;
        }
        
        return;
    }
    //========================
    
    //======Black_Jack=====
    if(bot->charater_ID == Black_Jack){

        get_card(draw(game),bot);
        Card *check = draw(game);
        
        SYS_BAR_PRINT("The Black_Jack's second card is %s",card_name(check->card_ID));
        get_card(check,bot);
        
        SYS_BAR_PRINT("%s use Black_Jack's skill and  he can draw another card!",bot->user_name);
        
        get_card(draw(game),bot);
        return;
    }
    //========================
    
    //======Kit_Carlson=====
    if(bot->charater_ID == Kit_Carlson){
        SYS_BAR_PRINT("%s use Kit_Carlson's skill to draw three cards and discard one!",bot->user_name);
        get_card(draw(game),bot);
        get_card(draw(game),bot);
        get_card(draw(game),bot);
        select_discard(game,bot);
        return;
    }
    //========================
    
    
    get_card(draw(game),bot);
    get_card(draw(game),bot);
    return;
}

void discard_stage(List *game,Player *bot)
{
    
    while(bot->cards_num > bot->bullets)
    {
        select_discard(game,bot);
    }
    SYS_BAR_PRINT("\033[1;34m%s end his turn \033[0m",bot->user_name);
    game->bang_play = 0;
    game->isInJail = 0;
    game->gear_change = 0;
    return;
}

// win message     ||     
//=================\/===

void win_message(List game)
{
    if(game.win_role == SHERIFF)
    {
        puts("YAAAAAAAAA \n SHERIFF AND DEPUTY SHERIFF WIN!!!!!!!!!!!\n OUTLAWS AND RENEGADE are dead!!!!");
    }
    else if(game.win_role == OUTLAW)
    {
        puts("AAAAAAAAAA \n OUTLAWS WINS!!!!!!!!!!!\n OTHERS are dead!!!!");
    }
    else if(game.win_role == RENEGADE)
    {
        puts("WOWOWOWOWO \n RENEGADE WINS!!!!!!!!!!!\n OTHERS are dead!!!!");
    }
    return;
}






