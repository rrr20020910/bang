#pragma once

#define CARD_NUM 80
#define CARD_TPYE 22
#define PILE_NUM 200

#define MAX_CARD_HOLD 18

#define MAX_GEAR_EQUIP 6

#define SHERIFF        1
#define DEPUTY_SHERIFF 2
#define OUTLAW         3
#define RENEGADE       4

#define BANG 101
#define BANG_NAME "Bang!"

#define MISS 102
#define MISS_NAME "Miss" 

#define BEER 103
#define BEER_NAME "Beer"

#define CAT_BLAUE 104
#define CAT_BLAUE_NAME "Cat_Balou"

#define PANIC 105
#define PANIC_NAME "Panic"

#define DUEL 106
#define DUEL_NAME "Duel"

#define GENERAL_STORE 107
#define GENERAL_STORE_NAME "General_Store"

#define INDIANS 108
#define INDIANS_NAME "Indians"

#define GATLING 109
#define GATLING_NAME "Gatling" 

#define SALON 110
#define SALON_NAME "Salon"

#define WELLS_FARGO 111
#define WELLS_FARGO_NAME "Wells_Fargo"

#define STAGECOACH 112
#define STAGECOACH_NAME "Stagecoach"

#define JAIL     213
#define JAIL_NAME "Jail"

#define DYNAMITE 214
#define DYNAMITE_NAME "Dynamite"

#define BARREL   215
#define BARREL_NAME "Barrel"

#define MUSTANG     216
#define MUSTANG_NAME "Mustang" 

#define APPALOOSA   217
#define APPALOOSA_NAME "Appaloosa" 

#define SCHOFIELD   218
#define SCHOFIELD_NAME "Schofield"

#define VOLANIC     219
#define VOLANIC_NAME "Volanic" 

#define REMINGTON   220
#define REMINGTON_NAME "Remington"

#define CARABINE    221
#define CARABINE_NAME "Rev.carabine"

#define WINCHESTER  222
#define WINCHESTER_NAME "Winchester" 

#define SIDE 0 //range == 0 means two sides

#define SYS_BAR_ROW 15
#define SYS_MSG_LENGTH 64

#define MAX_HAND_PRINT 6

#define CARD_WIDTH   17
#define CARD_LENGTH  12
#define CARD_IMG_FILEPATH "./assets/card-image"

#define LIMIT_OPTION_RANGE(var) (var<0||var>10)

#define SYS_BAR_ROW 15
#define SYS_MSG_LENGTH 64
#define SYS_BAR_PRINT(format, args...) ({ \
    char *_str = (char*)calloc(SYS_MSG_LENGTH,sizeof(char*)); \
    sprintf(_str,format, ##args); \
    print_system_msg(0,_str); \
    free(_str); \
})

#define INFO_MSG_PRINT(format, args...) ({ \
    printf(format"\n", ##args); \
})

#define WARNING_MSG_PRINT(format, args...) ({ \
    printf(format"\n", ##args); \
    usleep(2000000); \
})

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
    
    int32_t card_page;
    int32_t isInJail;

    int32_t pile_pos;
    Card pile[PILE_NUM];

    int32_t discard_pos;
    Card discard_pile[PILE_NUM];
    
    Card *current_card;
    Player *next;
}List;

