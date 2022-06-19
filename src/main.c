#include "utils/cstd.h"
#include "game.h"
#include "list.h"
#include "print.h"
#include "player.h"
#include "card.h"
#include "menu.h"

int main()
{   
    print_menu();
    int32_t player_num = 6;
    List game;
    memset(&game,0,sizeof(List));
    game.next = NULL;
    game.players_num = player_num;
    
    build_pile(&game);
    
    Player bot[player_num];
    
    //get_role
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
    for(int i=0;i<player_num;i++)
    {
        int32_t rand_pos = rand()%player_num;
        int32_t temp = role_list[i];
        role_list[i] = role_list[rand_pos];
        role_list[rand_pos] = temp;
    }
    
    //player_info
    for(int i=0;i<player_num;i++)
    {
        memset(&bot[i],0,sizeof(Player));
        set_player(&game,&bot[i]);
        build_list(&game,bot[i]);
    }
    set_role(&game);
    set_character(&game);
    
    game.next = get_sheriff(&game);
    Player *current = game.next;
    game.current_player = current;
    printf("bugger\n");
    
    while(1)
    {
        //stage 1 : draw 2 cards
        print_board(&game,current);
        draw_stage(&game,current);
        
        //stage 2 : use cards
        while(!game.isInJail)
        {
            print_board(&game,current);
            
            if(get_player_move(&game,current))
            {
                break;
            }
            
        }
        
        //win check
        if(game.win_role != 0)
        {
            break;
        }
        
        //stage 3 : discard
        discard_stage(&game,current);
        current = current->next;
        game.current_player = current;
    }
    
    win_message(game);
    
    free_list(&game);
    return 0;
}
