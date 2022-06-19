#include "menu.h"
#include <forward_declaration.h>

#define MAX_NAME_LEN 10

void start_of_the_game(List *game,char ***player_name)
{
    print_menu(game,player_name);
    return;
}

void menu_induction_control(List *game,char ***player_name)
{
    int32_t menu_induction_choice=0;
    printf("Please enter your choice: ");
    scanf("%d",&menu_induction_choice);
    fflush(stdin);
    switch(menu_induction_choice)
    {
        case 1:
        {
            menu_game_settings(game,player_name);
            return;
        }
        case 2:
        {
            printf("Gamemode.\n");
            //print_menu(game);
            return;
        }
        case 3:
        {
            print_rules(game,player_name);
            return;
        }
        case 4:
        {
            fflush(stdin); 
            printf("\nGood Bye!!\nENTER TO CONTINUE......"); 
            fgetc(stdin); 
            system("clear");
            exit(0);
        }
        default:
        {
            print_menu(game,player_name);

        }
    }
}

void print_menu(List *game,char ***player_name)
{
    FILE *pMenu;
    
    if((pMenu = fopen("./assets/menu-image.txt","r")) == NULL)
    {
        perror("Error: ");
        return;
    }

    char str_tmp[200];
    system("clear");
    while(fgets(str_tmp,sizeof(str_tmp),pMenu) != NULL)
    {
        if(str_tmp[strlen(str_tmp)-1] == '\n')
        {
            str_tmp[strlen(str_tmp)-1] = 0;
        }
        printf("%s\n",str_tmp);
    }
    fclose(pMenu);
    printf("\n\n\n\n\n");
    menu_induction_control(game,player_name);
    return;
}

void print_rules(List *game,char ***player_name)
{
    FILE *pRules;
    char Rule_str[200];
    int32_t Rule_page=1;
    int32_t Rule_page_choice=0;

    while(Rule_page != -1)
    {
        system("clear");
        if((pRules = fopen("./assets/Rule_book.txt","r")) == NULL)
        {
            perror("Error: ");
            return;
        }
        for(size_t i=0;i<(Rule_page-1)*57;i++)
        {
            fgets(Rule_str,sizeof(Rule_str),pRules);
        }
        for(size_t i=0;i<57;i++)
        {
            fgets(Rule_str,sizeof(Rule_str),pRules);
            if(Rule_str[strlen(Rule_str)-1] == '\n')
            {
                Rule_str[strlen(Rule_str)-1] = 0;
            }
            printf("%s\n",Rule_str);
        }
        fclose(pRules);
        printf("Please enter your choice: ");
        scanf("%d",&Rule_page_choice);
        fflush(stdin);
        switch(Rule_page_choice)
        {
            case 0: 
            {
                Rule_page=-1;
                break;
            } 
            case 1:
            {
                Rule_page--;
                break;
            }
            case 2:
            {
                Rule_page++;
                break;
            }
            default:
            {
                break;
            }
        }
        if(Rule_page > 9) Rule_page=1;
        if(Rule_page == 0) Rule_page=9;
    }
    print_menu(game,player_name);
    return;

}

void menu_game_settings(List *game,char ***player_name)
{
    ////////Number of Players
    int32_t player_number=0;
    printf("\n==================");
    printf("\nGAME SETTING MODE");
    printf("\n==================\n");
    printf("How many players would you like to play? (It is recommended to play in 3 to 6 players): ");
    while(1)
    {
        scanf("%d",&player_number);
        fflush(stdin);
        if(player_number > 6 || player_number < 3) 
        {
            printf("(It's better to play with 3 to 6 players......): ");
            continue;
        }
        break;
    }
    
    game->players_num = player_number;
    //printf("Player number: %d\n",game->players_num);

    *player_name = (char**)calloc(player_number,sizeof(char**));
    
    char *str_tmp = malloc(sizeof(char)*1000);
    printf("\n=================================\n");
    for(int i=0;i<player_number;i++)
    {
        (*player_name)[i] = (char*)calloc(MAX_NAME_LEN,sizeof(char*));
        while(1)
        {
            printf("Please enter the player%d's name: ",i+1);
            scanf("%s",str_tmp);
            fflush(stdin);
            if(strlen(str_tmp) > 10)
            {
                printf("Your name is too long!\n");
                continue;    
            }
            //printf("%s\n",str_tmp);
            strncpy((*player_name)[i],str_tmp,10);
            
            //printf("Player name: %s\n",(*player_name)[i]);
            break;
        }
    }
    free(str_tmp);
    //printf("Success.\n");
    return;
}