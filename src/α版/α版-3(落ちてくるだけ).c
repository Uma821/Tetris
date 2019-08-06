//テトリスα版-3
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
void dainyu(void); //nextを一つ上げる
int rakkahantei(void);
int field[21][12];
int block[4][4];
int next[4][4][4];
int hold[4][4][1];
int temp[4][4];
int rakka = 0;
int x=0, y=3;
int soko[10]={0,0,0,0,0,0,0,0,0,0};
double dtime=1.0;

int main(void) {
    time_t start_time;
    start_time = time(NULL);
    srand((unsigned)time(NULL));
    nextpush(1);
    nextpush(2);
    nextpush(3);
    mainclear();
    hyoji();
    while(1) {
        if(difftime(time(NULL), start_time) == dtime){
            start_time = time(NULL);
            printf("\033[2J");
            hyoji();
            x++;
            rakka = rakkahantei();
            if(rakka == 1){
                dainyu();
                x = 0;
                y = 3;
                rakka = 0;
                break;
            }
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
int rakkahantei(void){
    int xhantei;
    for(int i=0; i<10; i++){
        xhantei = soko[i];
        if(i-y>=0 && i-y<4 && x-(20-xhantei)>=0){
            if(block[x-(20-xhantei)][i-y] == 1){
                return 1;
            }
        }
    }
    return 0;
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
            next[i][j][0] = block[i][j] = next[i][j][1];
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
    //field + block
    for(int j=0;j<10;j++){
        if(x-a>=0 && j-y>=0 && x-a<4 && j-y<4){
            if(field[a][j+1] == 1 && block[x-a][j-y] == 1)
                printf("??");
            else if((field[a][j] == 1 && block[x-a][j-y] == 0) || (field[a][j] == 0 && block[x-a][j-y] == 1))
                printf("■");
            else
                printf("  ");
        }
        else{
            if(field[a][j] == 1)
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
	int i,j;
    for(i=0; i<21; i++){
        for(j=0; j<12; j++){
            field[i][j] = 0;
        }
    }
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
        	next[i][j][0] = block[i][j] = next[i][j][1];
        }
    }
}