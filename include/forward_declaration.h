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

#define MAX_HAND_PRINT 6

#define CARD_WIDTH   17
#define CARD_LENGTH  12

#define CARD_IMG_FILEPATH "./assets/card-image"

typedef struct _sCard Card;
typedef struct _sPlayer Player;
typedef struct _sList List;

typedef struct _sCard
{
    char name[32];
    int32_t card_ID;
    int32_t (*ability)(List*,Player*);
    int32_t suit;
    int32_t face;
    int32_t is_available;
}Card;

typedef struct _sPlayer
{
    char user_name[16];
    int32_t bullets;
    int32_t cards_num;
    Card hand_card[MAX_CARD_HOLD];
    
    int32_t gear_num;
    Card gear[MAX_GEAR_EQUIP];
    
    int32_t role_ID;
    int32_t charater_ID;
    
    struct _sPlayer *next;
}Player;

typedef struct _sList
{
    int32_t players_num;
    int32_t bang_play;
    int32_t win_role;
    int32_t pos;
    
    int32_t card_page;
    
    Card pile[CARD_NUM];
    Card *current_card;
    //Player *current_player;
    Player *next;
}List;


