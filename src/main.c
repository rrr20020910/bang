#include "utils/cstd.h"
#include "game.h"
#include "list.h"
#include "print.h"
#include "player.h"
#include "card.h"
#include "menu.h"

int main()
{   
    List game;
    memset(&game,0,sizeof(List));
    game.next = NULL;
    
    build_pile(&game);
    

    char **player_name = NULL;
    start_of_the_game(&game,&player_name);
    
    Player bot[game.players_num];
    
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
    for(int i=0;i<game.players_num;i++)
    {
        int32_t rand_pos = rand()%game.players_num;
        int32_t temp = role_list[i];
        role_list[i] = role_list[rand_pos];
        role_list[rand_pos] = temp;
    }

    printf("%s\n",player_name[1]);

    //player_info
    for(int i=0;i<game.players_num;i++)
    {
        memset(&bot[i],0,sizeof(Player));
        strncpy(bot[i].user_name,player_name[i],strlen(player_name[i]));
        build_list(&game,bot[i]);
    }


    set_role(&game);
    set_character(&game);
    
    game.next = get_sheriff(&game);
    Player *current = game.next;
    game.current_player = current;
    
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
