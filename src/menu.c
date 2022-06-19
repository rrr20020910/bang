#include "menu.h"


void start_of_the_game()
{
    print_menu();
    //print_rules();
    return;
}

void menu_induction_control()
{
    int32_t menu_induction_choice=0;
    printf("Please enter your choice: ");
    scanf("%d",&menu_induction_choice);
    switch(menu_induction_choice)
    {
        case 1:
        {
            printf("Game Settings.\n");
            return;
        }
        case 2:
        {
            printf("Gamemode.\n");
            print_menu();
            return;
        }
        case 3:
        {
            print_rules();
            return;
        }
        case 4:
        {
            system("clear");
            exit(0);
        }
        default:
        {
            printf("Error!\n");
            sleep(1);
            print_menu();
            return;
        }
    }
}

void print_menu()
{
    FILE *pMenu;
    
    if((pMenu = fopen("./assets/menu-image.txt","r")) == NULL)
    {
        perror("Error: ");
        return;
    }

    char str_tmp[100];
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
    menu_induction_control();
    return;
}

void print_rules()
{
    FILE *pRules;
    char Rule_str[200];
    int32_t Rule_page=1;
    int32_t Rule_page_choice=0;

    while(Rule_page != 0)
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
        switch(Rule_page_choice)
        {
            case 0: 
            {
                Rule_page=0;
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
        }
        if(Rule_page > 9) Rule_page=0;
    }
    print_menu();
    return;

}