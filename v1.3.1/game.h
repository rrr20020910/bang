#pragma once

#define CARD_NUM 70
#define CARD_TPYE 15

#define MAX_CARD_HOLD 18

#define MAX_GEAR_EQUIP 6

#define SHERIFF        1
#define DEPUTY_SHERIFF 2
#define OUTLAW         3
#define RENEGADE       4

#define JAIL     213
#define DYNAMITE 214
#define BARREL   215

typedef struct _player player;
typedef struct _list list;

typedef struct _card{
    char name[32];
    int card_ID;
    int (*ability)(list*,player*);
    int suit;
    int face;
    int is_available;
}card;


typedef struct _player{
    char user_name[16];
    int bullets;
    int cards_num;
    card hand_card[MAX_CARD_HOLD];
    
    int gear_num;
    card gear[MAX_GEAR_EQUIP];
    
    int role_ID;
    int charater_ID;
    
    struct _player *next;
}player;


typedef struct _list{
    int players_num;
    int bang_play;
    int win_role;
    int pos;
    
    int card_page;
    
    card pile[CARD_NUM];
    card *current_card;
    //player *current_player;
    player *next;
}list;


// game function
int available(list *game,player *bot);
card *draw(list *game);
void get_card(card *new_card,player *bot);
void set_player(list *game,player *bot);
int get_distance(player *bot1,player *bot2);
player *get_sheriff(list *game);
player *select_other_player(list *game,player *bot);


// print function
void print_player(player *bot);
void print_card(card crd);
void print_player_hand(player *bot);

// pile function
void build_pile(list *game);

//card ability
int play_specify_card(list *game,player *bot,int card_id);
card *discard(player *bot);

int gear_check(player *bot,int gear_ID);

//stage
void draw_stage(list *game,player *bot);

