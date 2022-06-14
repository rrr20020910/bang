#include<stdio.h>
#include<stdlib.h>
#include"game.h"

#define MAX_HAND_PRINT 6

#define CARD_WIDTH   17
#define CARD_LENGTH  12

 
void gotoxy(int x,int y){
    printf("%c[%d;%df",0x1B,y,x);
}

//must print frist
void print_frame(int x,int y,int w,int h){
    //print head
    gotoxy(x,y);
    printf("/");
    for(int i=0;i<(w-2);i++){
        printf("=");
    }
    printf("\\");
    //print body
    for(int i=0;i<(h-2);i++){
    gotoxy(x,y+i+1);
    printf("|");
    gotoxy(x+w-1,y+i+1);
    printf("|");
    }
    //print tail
    gotoxy(x,y+h-1);
    printf("\\");
    for(int i=0;i<(w-2);i++){
        printf("=");
    }
    printf("/\n");
    return;
}

//role 
void print_role(int x,int y,player *bot){
    
    if(bot->role_ID == SHERIFF){
        gotoxy(x,y);
        printf("__/\\__");
        gotoxy(x,y+1);
        printf("\\ \\/ /");
        gotoxy(x,y+2);
        printf("/_/\\_\\");
        gotoxy(x,y+3);
        printf("  \\/  ");
        return;
    }
    if(bot->role_ID == DEPUTY_SHERIFF){
        gotoxy(x,y);
        printf("  /\\");
        gotoxy(x,y+1);
        printf(" /\\/\\");
        gotoxy(x,y+2);
        printf(" \\/\\/");
        gotoxy(x,y+3);
        printf("  \\/  ");
        return;
    }
    if(bot->role_ID == OUTLAW){
        gotoxy(x,y);
        printf("_/v\\_");
        gotoxy(x,y+1);
        printf("('_')");
        gotoxy(x,y+2);
        printf(" /|\\L");
        gotoxy(x,y+3);
        printf("  /\\");
        return;
    }
    if(bot->role_ID == RENEGADE){
        gotoxy(x,y);
        printf("{_/\\_}");
        gotoxy(x,y+1);
        printf("(d  b)");
        gotoxy(x,y+2);
        printf(" (==)");
        gotoxy(x,y+3);
        printf("  \\/  ");
        return;
    }
    return;
}

//gear

void print_player_visual(int x,int y,player *bot){

    print_frame(x,y,35,13);
    
    //name
    gotoxy(x+2,y+1);
    printf("%s",bot->user_name);
    
    //charater
    gotoxy(x+2,y+2);
    printf("charater");
    
    //bullets
    for(int i=0;i<(bot->bullets);i++){
        gotoxy(x+3,y+4+i);
        printf("~==>");
    }
    
    //target sign
    int b_x = x+30, b_y = y-1;
    gotoxy(b_x+2,b_y);
    printf("___");
    gotoxy(b_x+1,b_y+1);
    printf("/ | \\");
    gotoxy(b_x,b_y+2);
    printf("(~ X ~)");
    gotoxy(b_x+1,b_y+3);
    printf("\\_|_/");
    
    //gear
    int g_x = x+21,g_y = y+3;
    
    for(int i = 0;i<6;i++){
        gotoxy(g_x,g_y+i);
        if(bot->gear_num > i){
            printf("[ gear ]");
        }
        else{
            printf("[ X ]");
        }
    }
    
    //role
    print_role(x+11,y+4,bot);
    /*
    int r_x = x+9,r_y = y+4;
    for(int i=0;i<4;i++){
        gotoxy(r_x,r_y+i);
        printf("[========]");
    }*/
    
    //avata
    int a_x = x,a_y = y-3;
    for(int i=0;i<3;i++){
        gotoxy(a_x,a_y+i);
        printf("[------]");
    }
    
    //hand
    int count = bot->cards_num;
    int over = 0;
    if(count > MAX_HAND_PRINT){
        over = (count - MAX_HAND_PRINT);
        count = MAX_HAND_PRINT;
    }
    int last_pos =0;
    for(int i=0;i<count;i++){
        gotoxy(x+i*4+3,y+9);
        printf(" _ ");
        gotoxy(x+i*4+3,y+10);
        printf("| |");
        gotoxy(x+i*4+3,y+11);
        printf("|_|");
        last_pos = i;
    }
    if(over){
        gotoxy(x+last_pos*4+6,y+11);
        printf("...[%d]\n",over);
    }
    
    //select num
    
    
    return;
}

void print_card_visual(int x,int y,int card_ID){
    FILE *pfile = NULL;
    if((pfile=fopen("card.txt","r"))==NULL){
        printf("can't open this file\n");
        return;
    }
    fseek(pfile,(CARD_WIDTH-1)*CARD_LENGTH*card_ID,SEEK_SET);
    
    for(int i=0;i<CARD_LENGTH;i++){
        char card_pic[CARD_WIDTH] = {0};
        fgets(card_pic,CARD_WIDTH,pfile);
        gotoxy(x,y+i);
        printf("%s",card_pic);
    }
    printf("\n");
    fclose(pfile);
    return;
}

void print_next_page(int x,int y){
    gotoxy(x,y);
    printf("|\\");
    gotoxy(x,y+1);
    printf("| \\");
    gotoxy(x,y+2);
    printf("|  \\");
    gotoxy(x,y+3);
    printf("|  /");
    gotoxy(x,y+4);
    printf("| /");
    gotoxy(x,y+5);
    printf("|/");
    gotoxy(x,y+6);
    printf("[_7_]");
}

void print_prev_page(int x,int y){
    gotoxy(x,y);
    printf(" |/|");
    gotoxy(x,y+1);
    printf(" / |");
    gotoxy(x,y+2);
    printf("/  |");
    gotoxy(x,y+3);
    printf("\\  |");
    gotoxy(x,y+4);
    printf(" \\ |");
    gotoxy(x,y+5);
    printf(" |\\|");
    gotoxy(x-1,y+6);
    printf("[_0_]");
}


void print_next_turn(int x,int y){
    gotoxy(x,y);
    printf(" \\=========>");
    gotoxy(x,y+1);
    printf("/  End_turn [_9_] ");
}

void print_text(int x,int y){
    
    for(int i=0;i<15;i++){
        gotoxy(x,y+i);
        printf("%2d] abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz",i+1);
    }
    return;
}

void print_board(player *bot){
    
    system("clear");
    
    //print ===
    gotoxy(1,3);
    for(int i=0;i<158;i++){
        printf("=");
    }
    gotoxy(2,2);
    printf(" current player : %s  character : aaaaaaaaaaaaaaaaa",bot->user_name);
    gotoxy(143,2);
    printf("exit [_other_]");
    
    //middle card
    print_card_visual(44,24,0);
    print_card_visual(62,24,0);
    
    //print other
    player *current = bot->next;
    int count = 1;
    int player_pos[5][2] ={{4,25},{4,8},{43,8},{82,8},{121,8}};
    
    while(current != bot){

        print_player_visual(player_pos[count-1][0], player_pos[count-1][1],current);
        gotoxy(player_pos[count-1][0]+27, player_pos[count-1][1]+12);
        printf("[_%d_]",count);
        
        current = current->next;
        count++;
    }
    
    //print player
    print_frame(3,39,112,14);
    
    for(int i=0;i<6;i++){
        if(i<(bot->cards_num)){
            print_card_visual(9+i*17,40,(bot->hand_card[i].card_ID)%100);
            gotoxy(14+i*17,51);
            printf("[_%d_]",i+1);
        }
        else{
            print_card_visual(9+i*17,40,0);
        }
    }
    
    //player info
    
    //avata
    for(int i=0;i<3;i++){
        gotoxy(118,40+i);
        printf("[------]");
    }
    gotoxy(126,42);
    printf("_______________________________");
    
    //bullets
    for(int i=0;i<(bot->bullets);i++){
        gotoxy(120,44+i);
        printf("~==>");
    }
    
    //role
    print_role(128,44,bot);
    
    
    //character
    gotoxy(118,51);
    printf("abcdefghijk");
    gotoxy(118,52);
    printf("abcdefghijkabcdefghijk");
    
    //gear
    for(int i = 0;i<6;i++){
        gotoxy(140,44+i);
        printf("[ X ]");
    }
    
    //other
    print_next_page(112,43);
    print_prev_page(2,43);
    print_next_turn(52,37);
    print_text(81,23);
    
    gotoxy(1,60);    
    return;
}



