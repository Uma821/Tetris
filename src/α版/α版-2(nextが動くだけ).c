//テトリスα版-2
#define dtime 1.0
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void hyoji(void);
int nextpush(int);
void nexthyoji(int);
void mainhyoji(int);
int turnblok(void);
void mainclear(void);
void dainyu(void);
int field[20][10];
int block[4][4];
int next[4][4][4];
int hold[4][4][1];
int temp[4][4];
int rakka = 0;
int x=0, y=0;

int main(void) {
    time_t start_time;
    start_time = time(NULL);
    srand((unsigned)time(NULL));
    int n=0;
    mainclear();
    nextpush(1);
    nextpush(2);
    nextpush(3);
    while(1) {
        if(difftime(time(NULL), start_time) == dtime){
            if(rakka == 1)
                dainyu();
            start_time = time(NULL);
            printf("\033[2J");
            hyoji();
            rakka = 1;
            n++;
            if(n == 20)
                break;
        }
    }
    return 0;
}

//配列回転の技
int turnblok(void){
 //セーブだよ
    for(int j=0;j<4;j++){
        for(int i=0;i<4;i++){
            temp[j][i] = block[j][i];
        }
    }
    //ブロック回転ダ
    for(int i = 0; i<4; i++) {
        for(int j = 0; j<4; j++) {
            block[i][j] = temp[3-j][i];
        }
    }
    return 1;
}

int nextpush(int n){
    int randamu = rand() % 7;
    //降りてくるピースの種類の配列
    int object[7][4][4]={ {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},
                           {{0,0,0,0},{0,1,0,0},{0,1,0,0},{0,1,1,0}},
                           {{0,0,0,0},{0,0,1,0},{0,0,1,0},{0,1,1,0}},
                           {{0,0,0,0},{0,0,0,0},{0,1,1,0},{1,1,0,0}},
                           {{0,0,0,0},{0,0,0,0},{1,1,0,0},{0,1,1,0}},
                           {{0,0,0,0},{0,0,0,0},{0,1,1,0},{0,1,1,0}},
                           {{0,0,0,0},{0,0,0,0},{0,1,0,0},{1,1,1,0}} };
    //詰める
    for(int j=0;j<4;j++){
        for(int i=0;i<4;i++){
            next[j][i][n] = object[randamu][j][i];
        }
    }
    return randamu;

}

void dainyu(void){
    int i, j;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            next[i][j][0] = next[i][j][1];
            next[i][j][1] = next[i][j][2];
            next[i][j][2] = next[i][j][3];
        }
    }
    nextpush(3);
}

void nexthyoji(int a){
    for(int j=0;j<4;j++){
        if(a<4){
            if(next[a][j][1]==1)
                printf("■");
            else
                printf("  ");
        }
        else if(a<8){
            if(next[a-4][j][2]==1)
                printf("■");
            else
                printf("  ");
        }
        else if(a<12){
            if(next[a-8][j][3]==1)
                printf("■");
            else
                printf("  ");
        }
    }
}

void mainhyoji(int a) {
    for(int j=0;j<10;j++){
        if(field[a][j] == 1)
            printf("■");
        else
            printf("  ");
    }
}

void hyoji(void) {
    for(int a=0;a<21;a++){
        if(a==0)
            printf(" -next-----");
        else if(a==4 || a==8 || a==12)
            printf(" ----------");
        else if(a<12){
            printf(" |");
            nexthyoji(a);
            printf("|");
        }
        else
            printf("           ");

        if(a==20)
            printf("----------------------");
        else{
            printf("|");
            mainhyoji(a);
            printf("|");
        }

        if(a==0)
            printf("-hold-----");
        else if(a==4)
            printf("----------");
        else if(a<4)
         printf("|        |");

        puts("");
    }
    fflush(stdout);
}

void mainclear(void) {
    for(int i=0; i<20; i++){
        for(int j=0; j<10; j++){
         field[i][j] = 0;
        }
    }
}