#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"game.h"


// available move  ||     
//=================\/===

int play_card(list *game,player *bot,int sel){
    
    if(sel > bot->cards_num){
        printf("fail play\n");
        return 0;
    }
    card *current = &(bot->hand_card[sel]);
    game->current_card = current;
    
    if(bot->hand_card[sel].ability(game,bot)){
        printf("fail play\n");
        return 0;
    }
    
    for(int i=sel;i<(bot->cards_num);i++){
        bot->hand_card[i] = bot->hand_card[i+1];
    }
    bot->cards_num--;
    
    memset(&(bot->hand_card[bot->cards_num]),0,sizeof(card));
    return 0;
}

int available(list *game,player *bot){

    //get select
    printf("please select a move:");
    int sel=0;
    scanf("%d",&sel);
    
    //1~6 play card
    if((sel >= 1) && (sel <= 6)){
    
        play_card(game,bot,sel+(game->card_page)*6-1);
    
    }
    
    //0,7 page control
    if(sel == 0){
        if(game->card_page == 0){
            printf("you can't do that");
            return 0;
        }
        game->card_page --;
        return 0;
    }
    if(sel == 7){
        /*
        if(game->card_page*6 < (bot->cards_num)){
            printf("you can't do that");
            return 0;
        }*/
        game->card_page ++;
        if(game->card_page > 3){
            game->card_page = 0;
        }
    }
    
    //9 leave
    if(sel == 9){
        return 1;
    }
    
    return 0;
}

// draw & get      ||     
//=================\/===


card *draw(list *game){
    int ori_pos = game->pos;
    game->pos++;
    return &game->pile[ori_pos];
}

void get_card(card *new_card,player *bot){
    bot->hand_card[bot->cards_num] = *new_card;
    bot->cards_num++;
    new_card->is_available = 0;
    return;
}

// set_player      ||     
//=================\/===

void set_player(list *game,player *bot){
    printf("please enter user name : ");
    char name[16];
    scanf("%s",name);
    strcpy(bot->user_name,name);

    bot->charater_ID = 401;
    bot->bullets = (bot->charater_ID)/100;
    
    for(int i=0;i<(bot->bullets);i++){
        get_card(draw(game),bot);
    }
    
    return;
}



// select          ||     
//=================\/===
player *select_other_player(list *game,player *bot){
    player *current = bot;
    
    printf("please select a player:");
    int sel = 0;
    scanf("%d",&sel);
    
    for(int i=0;i<sel;i++){
        current = current->next;
    }
    return current;
}

// distance        ||     
//=================\/===
// range == 0 means two sides

int compare(int a,int b){
    if(a > b) return b;
    return a;
}

int get_distance(player *bot1,player *bot2){
    int left = 0;
    int right = 0;
    player *current = bot1;
    while(current != bot2){
        current = current->next;
        left++;
    }
    while(current != bot1){
        current = current->next;
        right++;
    }
    return compare(right,left);
}

#define SIDE 0

player *select_range_player(list *game,player *bot,int range){
    
    player *select = select_other_player(game,bot);
    
    if(range == SIDE){
        if((select->next == bot) || (bot->next == select)){
            return select;
        }
        else{
            return NULL;
        }
    }
    if(get_distance(select,bot) > range){
        return NULL;
    }
    
    return select;
}


// sheriff         ||     
//=================\/===

player *get_sheriff(list *game){

    

    player *sheriff = game->next;
    
    
    
    while(sheriff->role_ID != SHERIFF){
        sheriff = sheriff->next;
    }
    
    printf("bugger1\n");
    
    return sheriff;
}

// print           ||     
//=================\/===

void print_player(player *bot){
    printf("%8s: ",bot->user_name);
    printf("bullet: %d card_num[%d] gear:",bot->bullets,bot->cards_num);
    for(int i=0;i<bot->gear_num;i++){
        printf("%s,",bot->gear[i].name);
    }
    printf("\n");
    return;   
}

void print_card(card crd){
    printf("Card: %s(%d,%d)\n",crd.name,crd.suit,crd.face);
    return;
}

void print_player_hand(player *bot){
    for(int i=0;i<(bot->cards_num);i++){
        printf("%d) ",i+1);
        print_card(bot->hand_card[i]);
    }
    return;
}




// DIE             ||     
//=================\/===

void die(list *game,player *bot){

    printf("%s Has been slain!!!\n",bot->user_name);
    
    if(bot == game->next){
        printf("sheriff DIE\n");
        if((game->players_num == 2) && (bot->next->role_ID == RENEGADE)){
            game->win_role = RENEGADE;
            return;
        }
        game->win_role = OUTLAW;
        return;
    }
    
    //delete
    player *delete = game->next;
    while(delete->next != bot){
        delete = delete->next;
    }
    player *temp = delete->next;
    delete->next = temp->next;
    free(temp);
    game->players_num--;
    
    //sheriff win check
    if((game->players_num == 1) || ((game->players_num == 2) && (bot->next->role_ID == DEPUTY_SHERIFF))){
        game->win_role = SHERIFF;
    }
    return;
}


// damg & heal     ||     
//=================\/===

void heal(player *bot){
    if(bot->bullets < (bot->charater_ID/100)){
        bot->bullets++;
    }
    return;
}

void damg(list *game,player *bot){

    bot->bullets--;
    if(bot->bullets <= 0){
        player *current = bot->next;
        for(int i=0;i<((game->players_num)-1);i++){
            printf("%s is dying do you want ",bot->user_name);
            if(play_specify_card(game,current,103)){
                heal(bot);
            }
            if(bot->bullets > 0){
                break;
            }
            current=current->next;
        }
    }
    if(bot->bullets <= 0){
        die(game,bot);
    }
    return;
}

// play card       ||     
//=================\/===

char *card_name(int card_ID){
    char *name[3] = {"Bang!","Miss","beer"};
    return name[(card_ID%100)-1];
} 

int play_specify_card(list *game,player *bot,int card_id){
    
    printf("play %s\n",card_name(card_id));
    
    print_player_hand(bot);
    printf("%d) I don't want to play\n",bot->cards_num);
    printf("please select a card:");
    int sel=0;
    scanf("%d",&sel);
    sel--;
    if(bot->hand_card[sel].card_ID != card_id){
        return 0;
    }
    
    for(int i=sel;i<(bot->cards_num);i++){
        bot->hand_card[i] = bot->hand_card[i+1];
    }
    bot->cards_num--;
    memset(&(bot->hand_card[bot->cards_num]),0,sizeof(card));
    return 1;
}


// discard         ||     
//=================\/===

card *discard(player *bot){
    srand(time(NULL));
    int random_card = rand()%(bot->cards_num); 
    
    card *discard_card = &bot->hand_card[random_card];
    
    for(int i=random_card;i<(bot->cards_num);i++){
        bot->hand_card[i] = bot->hand_card[i+1];
    }
    
    bot->cards_num--;
    memset(&(bot->hand_card[bot->cards_num]),0,sizeof(card));
    return discard_card;
}




// brown card      ||     
//=================\/===

int Bang(list *game,player *bot){

    if(game->bang_play != 0){
        printf("you already play bang\n");
        return 1;
    }
    
    if(game->bang_play != 0){
        printf("you already play bang\n");
        return 1;
    }
    
    int Miss_flag = 0;
    
    printf("Bang! select a target\n");
    player *target = select_range_player(game,bot,1);
    if(target == NULL){
        printf("It's too far\n");
        return 1;
    }
    
    if(gear_check(target,BARREL)){
        printf("Miss! use BARREL\n");
        Miss_flag++;
    }
    
    if(play_specify_card(game,target,102)){
        Miss_flag++;
    }
    
    if(!Miss_flag){
        damg(game,target);
    }
    
    game->bang_play++;
    return 0;
}

int Wells_Fargo(list *game,player *bot){
    get_card(draw(game),bot);
    get_card(draw(game),bot);
    get_card(draw(game),bot);
    return 0;
}

int Stagecoach(list *game,player *bot){
    get_card(draw(game),bot);
    get_card(draw(game),bot);
    return 0;
}

int Salon(list *game,player *bot){
    player *current = bot;
    do{
        heal(current);
        current=current->next;
    }while((current) != bot);
    return 0;
}




int Miss(list *game,player *bot){
    printf("you can't player miss\n");
    return 1;
}

int Beer(list *game,player *bot){
    if(game->players_num != 2){
        heal(bot);
    }
    return 0;
}

int Panic(list *game,player *bot){
    player *target = select_other_player(game,bot);
    get_card(discard(target),bot);
    return 0;
}

int General_Store(list *game,player *bot){
    card *store[game->players_num];
    for(int i=0;i<(game->players_num);i++){
        store[i] = draw(game);
    }
    player *current = bot;
    for(int i=0;i<(game->players_num)-1;i++){
        //list card
        for(int j=0;j<(game->players_num)-i;j++){
            printf("%d) ",j+1);
            print_card(*store[j]);
        }
        printf("please select a card(General_Store): ");
        int sel = 0;
        scanf("%d",&sel);
        sel--;
        get_card(store[sel],current);
        
        for(int j=sel;j<(game->players_num)-i;j++){
            store[j] = store[j+1];
        }
        current=current->next;
    }
    get_card(store[0],current);
    return 0;
}

int Duel(list *game,player *bot){
    player *target = select_other_player(game,bot);
    player *current = target;
    
    while(1){
        if(!play_specify_card(game,current,101)){
            damg(game,current);
            break;
        }
        if(current == bot){
            current=target;
        }
        else{
            current=bot;
        }
    }
    return 0;
}

int Gatling(list *game,player *bot){
    player *current = bot;
    current = current->next;
    
    while(current != bot){
        if(!play_specify_card(game,current,102)){
            damg(game,current);
        }
        current=current->next;
    }
    return 0;
}

int Indians(list *game,player *bot){
    player *current = bot;
    current = current->next;
    
    while(current != bot){
        if(!play_specify_card(game,current,101)){
            damg(game,current);
        }
        current=current->next;
    }
    return 0;
}



/*
int Cat_Balou(list *game,player *bot){

1) random handcard
2) gun if thy have one
x)...list target gear

need card_name function

char* (int card_ID){

}

    player *target = select_player(game);
    int count = 0;
    
    int card_flag = 0;
    int gun_flag = 0;
    int gear_flag = 0;
    
    if(bot->cards_num){
        count++;
        card_flag++;
        printf("%d) discard %s random hand card\n",count,bot->user_name);
    }
    if(bot->gun){
        count++;
        gun_flag++;
        printf("%d) discard %s gun\n",count,bot->user_name);
    }
    for(int i=0;i<bot->gear_num;i++){
        count++;
        gear_flag++;
        printf("%d) discard %s %s\n",count,bot->user_name,card_name(bot->gear[i]));
    }
    printf("Please enter selection : ");
    int sel = 0 ;
    scanf("%d",&sel);
    
    if(sel == card_flag){
    
    }
    if(sel == gun_flag){
    
    }
    discard_random(target);
}
*/

// gear check      ||     
//=================\/===

int gear_check(player *bot,int gear_ID){
    for(int i=0;i<bot->gear_num;i++){
        if(bot->gear[bot->gear_num].card_ID == gear_ID){
            return i+1;
        }
    }
    return 0;
}

// discard gear    ||     
//=================\/===

card *discard_gear(player *bot,int gear_ID){
    
    int pos = gear_check(bot,gear_ID)-1;
    card *discard_card = &bot->gear[pos];
    
    for(int i=pos;i<(bot->gear_num);i++){
        bot->gear[i] = bot->gear[i+1];
    }
    bot->gear_num--;
    memset(&(bot->gear[bot->gear_num]),0,sizeof(card));
    
    return discard_card;
}



// blue card       ||     
//=================\/===

int Jail(list *game,player *bot){

    printf("Jail select a target\n");
    player *target = select_other_player(game,bot);
    
    if(gear_check(target,game->current_card->card_ID)){
        return 1;
    }
    
    card *equip = game->current_card;
    target->gear[target->gear_num] = *equip;
    target->gear_num ++;
    
    return 0;
}

int Dynamite(list *game,player *bot){

    bot->gear[bot->gear_num] = *game->current_card;
    bot->gear_num ++;
    
    return 0;
}

int Barrel(list *game,player *bot){
    
    if(gear_check(bot,game->current_card->card_ID)){
        printf("you already have %s",game->current_card->name);
        return 1;
    }
    bot->gear[bot->gear_num] = *game->current_card;
    bot->gear_num ++;
    
    return 0;
}


// pile            ||     
//=================\/=== 

void build_pile(list *game){
    
    card pile[CARD_TPYE] ={{"Bang!"         ,101,&Bang},
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
                           {"Barrel"        ,215,&Barrel}};
                    
    int every_card_num[CARD_TPYE] = {25,12,6,4,4,3,2,2,1,1,1,2,3,1,3};
    
    int count=0;
    for(int i=0;i<CARD_TPYE;i++){
        for(int j=0;j<every_card_num[i];j++){
            game->pile[count] = pile[i];
            count++;
        }
    }
    
    srand(time(NULL));
    
    for(int i=0;i<CARD_NUM;i++){
        game->pile[i].suit = rand()%4+1;
        game->pile[i].face = rand()%13+1;
    }
    
    for (int i=0;i<CARD_NUM;i++){
        int rand_pos = rand()%CARD_NUM;
        card temp = game->pile[i];
        game->pile[i] = game->pile[rand_pos];
        game->pile[rand_pos] = temp;
    }
    game->pos = 0;
    return;
}

// Stage           ||     
//=================\/=== 

void draw_stage(list *game,player *bot){

    game->card_page = 0;
    
    if(gear_check(bot,JAIL)){
        bot = bot->next;
        discard_gear(bot,JAIL);
    }
    
    if(gear_check(bot,DYNAMITE)){
        damg(game,bot);
        discard_gear(bot,DYNAMITE);
        Dynamite(game,bot->next);
    }
    
    get_card(draw(game),bot);
    get_card(draw(game),bot);
}








