#include "utils/cstd.h"
#include"game.h"
#include"list.h"
#include"player.h"
#include"print.h"

int main()
{
    int32_t player_num = 6;  // TODO: option num of players

    // TODO: game init function
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
        bot[i].role_ID = role_list[i];
        build_list(&game,bot[i]);
    }
    
    game.next = get_sheriff(&game);
    Player *current = game.next;
    
    while(1) // TODO: game loop function
    {
        //stage 1 : draw 2 cards
        draw_stage(&game,current);
        
        //stage 2 : use cards
        while(1)
        {
            print_board(current);
            if(available(&game,current))
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
        current = current->next;
        game.bang_play = 0;
    }
    
    // TODO: game end function
    free_list(&game);
    return 0;
}
