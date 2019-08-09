//テトリスver1.1 for Windows
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

int hyoji(void);
int nextpush(int);
void nexthyoji(int);
int mainhyoji(int);
int turnblok(void);
void gturn(void);
void mainclear(void);
void dainyu(void); //nextを一つ上げる
int linekeshi(void);
int teityaku(void);
int idou(void);
int holdhyoji(int);
int holdin(void);
int game(void);
int field[20][12];
int block[4][4];
int next[4][4][4];
int hold[4][4];
int temp[4][4];
int holdtemp[4][4];
int x=0, y=3;
int soko[10]={0,0,0,0,0,0,0,0,0,0};
double dtime=1.0;
int holdnow = 0;
int linekazu = 0;

int main(void) {
    int gcon = 1;
    time_t start_time = clock();
    srand((unsigned)time(NULL));
    nextpush(1);
    nextpush(2);
    nextpush(3);
    mainclear();
    dainyu();
    hyoji();
    while(gcon) {
        if(kbhit())
            idou();
        if((double)(clock() - start_time) / CLOCKS_PER_SEC > dtime){
            start_time = clock();
            x++;
            if(hyoji()){
                x++;
                teityaku();
                gcon = game();
                x = 0;
                y = 3;
                dainyu();
                linekeshi();
                x++;
                hyoji();
            }
        }
    }
    puts("\033[2J\033[0;0f\033[31mGemeOver\033[m");
    return 0;
}

int game(void){
    for(int i=1; i<12; i++)
        if(field[0][i])
            return 0;
    return 1;
}

int linekeshi(void){
    //複数行揃ったら消す
    do{
        linekazu = 0;
        int i, j, n, bou[2];
        for(i=19; i>=0; i--)
            for(j=1; j<11; j++){
                if(!field[i][j]){
                    if(linekazu > 0){
                        bou[1] = i;
                        goto end;
                    }
                    break;
                }
                if(j == 10){
                    if(!linekazu)
                        bou[0] = i;
                    linekazu++;
                }
            }
        end:
            if(linekazu > 0)
                for(j=bou[0]; j>=bou[0]-bou[1]; j--)
                    for(n=1; n<11; n++)
                        field[j][n] = field[j-(bou[0]-bou[1])][n];

    }while(linekazu);
    return 0;
}

int idou(void){
    char h = getch();
    if(h == 'l'){
        right:
        y++;
        if(hyoji()){
            y--;
            hyoji();
        }
    }
    else if(h == 'k'){
        left:
        y--;
        if(hyoji()){
            y++;
            hyoji();
        }
    }
    else if(h == 'm'){
        down:
        x++;
        if(hyoji()){
            x--;
            hyoji();
        }
    }
    else if(h == 'j'){
        turn:
        turnblok();
        if(hyoji()){
            gturn();
            hyoji();
        }
    }
    else if(h == 'h'){
        holdin();
        x = 0;
        y = 3;
    }
    //矢印キー入力①
    else if(h == '\xe0'){
        h = getch();
        if(h == '\x4D')
            goto right;
        else if(h == '\x4B')
            goto left;
        else if(h == '\x50')
            goto down;
        else if(h == '\x48')
            goto turn;
    }
    //矢印キー入力②
    else if(h == '\x1b' && getch() == '\x5b'){
        h = getch();
        if(h == '\x43')
            goto right;
        else if(h == '\x44')
            goto left;
        else if(h == '\x42')
            goto down;
        else if(h == '\x41')
           goto turn;
    }
    return 0;
}

int holdin(void){
    int i, j;
    //holdに入れる
    if(!holdnow){
        for(j=0; j<4; j++)
            for(i=0;i<4;i++)
                hold[j][i] = next[j][i][0];
        holdnow = 1;
        dainyu();
    }
    else{
        for(j=0; j<4; j++)
            for(i=0; i<4; i++)
                holdtemp[j][i] = next[j][i][0];
        for(j=0; j<4; j++)
            for(i=0; i<4; i++)
                next[j][i][0] = block[j][i] = hold[j][i];
        for(j=0; j<4; j++)
            for(i=0; i<4; i++)
                hold[j][i] = holdtemp[j][i];
    }
    return 0;
}

void gturn(void){
    //配列回転の技でセーブしたtemp配列をblock配列に入れ直す→逆回転
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            block[i][j] = temp[i][j];
}

//配列回転の技
int turnblok(void){
    int i, j;
    //セーブだよ
    for(j=0; j<4; j++)
        for(i=0; i<4; i++)
            temp[j][i] = block[j][i];
    //ブロック回転ダ
    for(i = 0; i<4; i++)
        for(j = 0; j<4; j++)
            block[i][j] = temp[3-j][i];
    return 1;
}

int teityaku(void){
    //地面に接触したものをfield配列に定着させる
    for(int i=20; i>=0; i--)
        for(int j=11; j>=0; j--)
            if(x-1-3<=i && x-1>=i && y<=j && y+3>=j && block[3-(x-i)][3-(y-1-j)])
                field[i-1][j+1] = block[3-(x-i)][3-(y-1-j)];
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
    for(int j=0; j<4; j++)
        for(int i=0; i<4; i++)
            next[j][i][n] = object[randamu][j][i];
    return randamu;

}

void dainyu(void){
    //一個分押し上げる
    int i, j;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++){
            next[i][j][0] = block[i][j] = next[i][j][1];
            next[i][j][1] = next[i][j][2];
            next[i][j][2] = next[i][j][3];
        }
    nextpush(3);
}

void nexthyoji(int a){
    //next部分だけの表示を任された関数
    for(int j=0; j<4; j++){
        if(a<4){
            if(next[a][j][1])
                printf("■");
            else
                printf("  ");
        }
        else if(a<8){
            if(next[a-4][j][2])
                printf("■");
            else
                printf("  ");
        }
        else if(a<12){
            if(next[a-8][j][3])
                printf("■");
            else
                printf("  ");
        }
    }
}

int mainhyoji(int a) {
    int n = 20;
    //field + block
    //main部分だけの表示を任された関数
    //[3-(x-a)]なのはそのままやると上下が反転するから
    for(int j=0; j<12; j++){
        if(j>0 && j<11){
            if(x-a>=0 && j-y-1>=0 && x-a<4 && j-y-1<4){
                if(field[a][j] && block[3-(x-a)][j-y-1]){
                    printf("??");
                    return 1;
                }
                else if((field[a][j] && !block[3-(x-a)][j-y-1]) || (!field[a][j] && block[3-(x-a)][j-y-1]))
                    printf("■");
                else
                    printf("  ");
            }
            else{
                if(field[a][j])
                    printf("■");
                else
                    printf("  ");
            }
            if(x-n>=0 && j-y-1>=0 && x-n<4 && j-y-1<4 && block[3-(x-n)][j-y-1])
                    return 1;
        }
        else{
            if(x-a>=0 && j-y-1>=0 && x-a<4 && j-y-1<4 && block[3-(x-a)][j-y-1])
                    return 1;
            if(x-n>=0 && j-y-1>=0 && x-n<4 && j-y-1<4 && block[3-(x-n)][j-y-1])
                    return 1;
        }
    }
    return 0;
}

int holdhyoji(int a){
    //hold部分だけの表示を任された関数
    for(int j=0; j<4; j++)
        if(a<4){
            if(hold[a][j])
                printf("■");
            else
                printf("  ");
        }
    return 0;
}

int hyoji(void) {
    //枠を表示する中身はほかの関数に任せる
    printf("\033[2J\033[0;0f");
    for(int a=0; a<21; a++){
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
            if(mainhyoji(a))
                return 1;
            printf("|");
        }

        if(a==0)
            printf("-hold-----");
        else if(a==4)
            printf("----------");
        else if(a<4){
            printf("|");
            holdhyoji(a);
            printf("|");
        }

        puts("");
    }
    fflush(stdout);
    return 0;
}

void mainclear(void) {
    //field配列を0にリセットしておく
    int i, j;
    for(i=0; i<20; i++)
        for(j=0; j<12; j++)
            field[i][j] = 0;
    /*nextpush関数でnext[][][1]に落ちてくるやつを設定しているので
    それを実際に落ちてくる配列blockと隠し配列next[][][0]にセットする*/
    for(i=0; i<4; i++)
        for(j=0; j<4; j++){
            next[i][j][0] = block[i][j] = next[i][j][1];
            hold[i][j] = 0;
        }
}