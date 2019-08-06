//テトリスα版-1
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void hyoji(void);
int nextpush(int);
void nexthyoji(int);
int turnblok(void);
int field[20][10];
int block[4][4];
int next[4][4][4];
int hold[3][4][1];

int main(void) {
    srand((unsigned)time(NULL));
    nextpush(1);
    nextpush(2);
    nextpush(3);
    for(int n=0;n<100;n++) {
        usleep(1000000);
        printf("\033[2J");
        hyoji();
    }
    return 0;
}
int turnblok(void){

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

void nexthyoji(int a){
    for(int j=0;j<4;j++){
        if(a<4){
            if(next[a][j][2]==1)
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
        else
            printf("|                    |");

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