//テトリスver0.8
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

int hyoji(void);
int nextpush(int);
void nexthyoji(int);
void mainhyoji(int);
int turnblok(void);
void mainclear(void);
void dainyu(void); //nextを一つ上げる
int rakkahantei(void);
void sokoshirabe(void);
int field[21][12];
int block[4][4];
int next[4][4][4];
int hold[4][4][1];
int temp[4][4];
int rakka = 0;
int rakka2 = 0;
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
    dainyu();
    while(1) {
        if(difftime(time(NULL), start_time) == dtime){
            start_time = time(NULL);
            printf("\033[2J");
            rakka2 = hyoji();
            x++;
            rakka = rakkahantei();
            if(rakka == 1 || rakka2 == 1){
            	sokoshirabe();
                dainyu();
                x = 0;
                y = 3;
                rakka = 0;
                rakka2 = 0;
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
void sokoshirabe(void){
    //地面に接触したものをfield配列に定着させる
    //底を調べているsoko配列を更新する２つの役割がある

    for(int i=19; i>=0; i--){
        if(x-1-3<=i && x-1>=i){
            for(int j=11; j>=1; j--){
                if(y<=j && y+3>=j){
                    if(block[3-(x-i)][3-(y-1-j)] == 1){
                        if(rakka2 == 1){
                            field[i-1][j+1] = block[3-(x-i)][3-(y-1-j)];
                        }
                        else{
                            field[i][j+1] = block[3-(x-i)][3-(y-1-j)];
                        }
                    }
                }
            }
        }

    }puts("\n");
    for(int i=1; i<11; i++){
        for(int j=0; j<20; j++){
            if(field[j][i] == 1){
                soko[i-1] = 19-j;
                break;
            }
        }
    }

}
int rakkahantei(void){
    //落下判定を行います、でもそんなに使いません
    int xhantei;
    for(int i=0; i<10; i++){
        xhantei = soko[i];
        if(i-y>=0 && i-y<4 && 3-(x-(20-xhantei))>=0 && 3-(x-(20-xhantei))<4){
            if(block[3-(x-(20-xhantei))][i-y] == 1){
                return 1;
            }
        }
    }
    return 0;
}

int nextpush(int n){
    //乱数でパーツを設定しています
    int randamu = rand() % 7;
    //降りてくるピースの種類の配列
    int object[8][4][4]={ {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},
                           {{0,0,0,0},{0,1,0,0},{0,1,0,0},{0,1,1,0}},
                           {{0,0,0,0},{0,0,1,0},{0,0,1,0},{0,1,1,0}},
                           {{0,0,0,0},{0,0,0,0},{0,1,1,0},{1,1,0,0}},
                           {{0,0,0,0},{0,0,0,0},{1,1,0,0},{0,1,1,0}},
                           {{0,0,0,0},{0,0,0,0},{0,1,1,0},{0,1,1,0}},
                           {{0,0,0,0},{0,0,0,0},{0,1,0,0},{1,1,1,0}},
                           {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}} };
    //詰める
    for(int j=0;j<4;j++){
        for(int i=0;i<4;i++){
            next[j][i][n] = object[randamu][j][i];
        }
    }
    return randamu;

}

void dainyu(void){
    //一個分押し上げる
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
    //next部分だけの表示を任された関数
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
    //main部分だけの表示を任された関数
    //[3-(x-a)]なのはそのままやると上下が反転するから
    for(int j=1;j<11;j++){
        if(x-a>=0 && j-y-1>=0 && x-a<4 && j-y-1<4){
            if(field[a][j] == 1 && block[3-(x-a)][j-y-1] == 1){
                printf("??");
                printf("\033[2J");
                rakka2 = 1;
            }
            else if((field[a][j] == 1 && block[3-(x-a)][j-y-1] == 0) || (field[a][j] == 0 && block[3-(x-a)][j-y-1] == 1))
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

int hyoji(void) {
    //枠を表示する中身はほかの関数に任せる
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
            if (rakka2 == 1){
                printf("\033[2J");
                return 1;
            }
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
    return 0;
}

void mainclear(void) {
    //field配列を0にリセットしておく
	int i,j;
    for(i=0; i<21; i++){
        for(j=0; j<12; j++){
            field[i][j] = 0;
        }
    }
    /*nextpush関数でnext[][][1]に落ちてくるやつを設定しているので
    それを実際に落ちてくる配列blockと隠し配列next[][][0]にセットする*/
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
        	next[i][j][0] = block[i][j] = next[i][j][1];
        }
    }
}
