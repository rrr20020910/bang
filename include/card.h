#pragma once

#include "utils/cstd.h"
#include "forward_declaration.h"
#include "player.h"
#include "list.h"

char *card_name(int32_t card_ID);

void damg(List *game,Player *bot);

int32_t Dynamite(List *game,Player *bot);

// pile function
void build_pile(List *game);

// old version code below
// ==================================
// typedef struct _sCard
// {
//     uint8_t suit; // size: 4 
//     uint16_t value; // size: 13
//     uint32_t item; //size: 22
// }Card;

// void bart_cassidy_ability(void);
// void black_jack_ability(void);
// void calamity_ability(void);
// void el_gringo_ability(void);
// void jesse_jones_ability(void);
// void jourdonnais_ablility(void);
// void kit_carlson_ability(void);
// void lucky_duke_ability(void);
// void paul_regret_ability(void);
// void pedro_ramirez_ability(void);
// void rose_doolan_ability(void);
// void sid_ketchum_ability(void);
// void slab_the_killer_ability(void);
// void suzy_lafayette(void);
// void vulture_sam(void);
// void willy_the_kid(void);

// //return false if card does not successfully used
// //Otherwise return true

// //Gun
// bool schofield_ability(void);
// bool volcanic_ability(void);
// bool remington_ability(void);
// bool rev_carabine_ability(void);
// bool winchester_ability(void);

// //Horse
// bool mustang_ability(void);
// bool appaloosa_ability(void);

// //Other
// bool jail_ability(void);
// bool dynamite_ability(void);
// bool barrel_ability(void);

// //Brown
// bool bang_ability(void);
// bool miss_ability(void);
// bool beer_ability(void);
// bool cat_balou_ability(void);
// bool panic_ability(void);
// bool duel_ability(void);
// bool general_store_ability(void);
// bool indians_ability(void);
// bool stagecoach_ability(void);
// bool gatling_ability(void);
// bool saloon_ability(void);
// bool wells_fargo_ability(void);


// void sheriff_ability(void);
// void deputy_sheriff_ability(void);
// void outlaw_ability(void);
// void renegade_ability(void);
// ===================================
