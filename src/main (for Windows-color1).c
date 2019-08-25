//テトリスver1.3 for Windows-color1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define tomato printf("\033[38;2;255;104;77m■\033[m");
#define yellow printf("\033[38;2;255;212;64m■\033[m");
#define lightblue printf("\033[38;2;119;192;199m■\033[m");
#define pink printf("\033[38;2;240;115;159m■\033[m");
#define green printf("\033[38;2;118;179;57m■\033[m");
#define orange printf("\033[38;2;251;161;64m■\033[m");
#define blue printf("\033[38;2;71;140;195m■\033[m");

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
double kaizyo(double, int);
int score = 0;
int field[2][20][12];
int next[4][4][4];
int hold[4][4];
int temp[4][4];
int holdtemp[4][4];
int x=0, y=3;
int soko[10]={0,0,0,0,0,0,0,0,0,0};
double dtime=1.0;
int holdnow = 0;
int linekazu = 0;

struct block {
    int block[4][4];
    int bcolor;
    int hcolor;
    int htcolor;
    int ncolor[4];
};

struct block block;

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
    printf("あなたの得点は %d点です\n",score);
    return 0;
}

int game(void){
    for(int i=1; i<12; i++)
        if(field[0][0][i])
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
                if(!field[0][i][j]){
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
            if(linekazu > 0){
                for(j=bou[0]; j>=bou[0]-bou[1]; j--)
                    for(n=1; n<11; n++){
                        field[0][j][n] = field[0][j-(bou[0]-bou[1])][n];
                        field[1][j][n] = field[1][j-(bou[0]-bou[1])][n];
                    }
                score = score + 10*linekazu*linekazu;
                dtime = kaizyo(0.997, score/10);
            }
    }while(linekazu);
    return 0;
}

double kaizyo(double s, int n){
    if(n == 1)
        return s;
    return s*kaizyo(s, n-1);
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
        block.hcolor = block.ncolor[0];
        holdnow = 1;
        dainyu();
    }
    else{
        for(j=0; j<4; j++)
            for(i=0; i<4; i++)
                holdtemp[j][i] = next[j][i][0];
        block.htcolor = block.ncolor[0];
        for(j=0; j<4; j++)
            for(i=0; i<4; i++)
                next[j][i][0] = block.block[j][i] = hold[j][i];
        block.ncolor[0] = block.bcolor = block.hcolor;
        for(j=0; j<4; j++)
            for(i=0; i<4; i++)
                hold[j][i] = holdtemp[j][i];
        block.hcolor = block.htcolor;
    }
    return 0;
}

void gturn(void){
    //配列回転の技でセーブしたtemp配列をblock.block配列に入れ直す→逆回転
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            block.block[i][j] = temp[i][j];
}

//配列回転の技
int turnblok(void){
    int i, j;
    //セーブだよ
    for(j=0; j<4; j++)
        for(i=0; i<4; i++)
            temp[j][i] = block.block[j][i];
    //ブロック回転ダ
    if(block.bcolor == 0)
        for(i = 0; i<4; i++)
            for(j = 0; j<4; j++)
                block.block[i][j] = temp[3-j][i];
    else if(block.bcolor==2 || block.bcolor==2)
        for(i=0; i<3; i++)
            for(j=0; j<3; j++)
                block.block[i+1][j+1] = temp[3-j][i+1];
    else if(block.bcolor != 5)
        for(i=0; i<3; i++)
            for(j=0; j<3; j++)
                block.block[i+1][j] = temp[3-j][i];
    return 1;
}

int teityaku(void){
    //地面に接触したものをfield[0]配列に定着させる
    for(int i=20; i>=0; i--)
        for(int j=11; j>=0; j--)
            if(x-1-3<=i && x-1>=i && y<=j && y+3>=j && block.block[3-(x-i)][3-(y-1-j)]){
                field[0][i-1][j+1] = block.block[3-(x-i)][3-(y-1-j)];
                field[1][i-1][j+1] = block.bcolor;
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
                           {{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
                           {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}} };
    //詰める
    for(int j=0; j<4; j++)
        for(int i=0; i<4; i++)
            next[j][i][n] = object[randamu][j][i];
    block.ncolor[n] = randamu;
    return randamu;
}

void dainyu(void){
    //一個分押し上げる
    int i, j;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++){
            next[i][j][0] = block.block[i][j] = next[i][j][1];
            next[i][j][1] = next[i][j][2];
            next[i][j][2] = next[i][j][3];
        }
    block.ncolor[0] = block.bcolor = block.ncolor[1];
    block.ncolor[1] = block.ncolor[2];
    block.ncolor[2] = block.ncolor[3];
    nextpush(3);
}

void nexthyoji(int a){
    //next部分だけの表示を任された関数
    for(int j=0; j<4; j++){
        if((block.ncolor[a/4+1]==6 && next[(a-1)%4][j][a/4+1]) || (block.ncolor[a/4+1]!=6 && next[a%4][j][a/4+1])){
            if(block.ncolor[a/4+1] == 0)
                tomato
            else if(block.ncolor[a/4+1] == 1)
                yellow
            else if(block.ncolor[a/4+1] == 2)
                lightblue
            else if(block.ncolor[a/4+1] == 3)
                pink
            else if(block.ncolor[a/4+1] == 4)
                green
            else if(block.ncolor[a/4+1] == 5)
                orange
            else if(block.ncolor[a/4+1] == 6)
                blue
        }
        else
            printf("  ");
    }
}

int mainhyoji(int a) {
    int n = 20;
    //field[0] + block.block
    //main部分だけの表示を任された関数
    //[3-(x-a)]なのはそのままやると上下が反転するから
    for(int j=0; j<12; j++){
        if(j>0 && j<11){
            if(x-a>=0 && j-y-1>=0 && x-a<4 && j-y-1<4 && field[0][a][j] && block.block[3-(x-a)][j-y-1])
                return 1;
            else if(x-a>=0 && j-y-1>=0 && x-a<4 && j-y-1<4 && !field[0][a][j] && block.block[3-(x-a)][j-y-1]){
                if(block.bcolor == 0)
                    tomato
                else if(block.bcolor == 1)
                    yellow
                else if(block.bcolor == 2)
                    lightblue
                else if(block.bcolor == 3)
                    pink
                else if(block.bcolor == 4)
                    green
                else if(block.bcolor == 5)
                    orange
                else if(block.bcolor == 6)
                    blue
            }
            else if(field[0][a][j] && ((x-a<0 || j-y-1<0 || x-a>=4 || j-y-1>=4) || (x-a>=0 && j-y-1>=0 && x-a<4 && j-y-1<4 && !block.block[3-(x-a)][j-y-1]))){
                if(field[1][a][j] == 0)
                    tomato
                else if(field[1][a][j] == 1)
                    yellow
                else if(field[1][a][j] == 2)
                    lightblue
                else if(field[1][a][j] == 3)
                    pink
                else if(field[1][a][j] == 4)
                    green
                else if(field[1][a][j] == 5)
                    orange
                else if(field[1][a][j] == 6)
                    blue
            }
            else
                printf("  ");
        }
        if(((j<1 || j>10) && x-a>=0 && j-y-1>=0 && x-a<4 && j-y-1<4 && block.block[3-(x-a)][j-y-1]) || (x-n>=0 && j-y-1>=0 && x-n<4 && j-y-1<4 && block.block[3-(x-n)][j-y-1]))
            return 1;
    }
    return 0;
}

int holdhyoji(int a){
    //hold部分だけの表示を任された関数
    for(int j=0; j<4; j++)
        if(a<4){
            if((block.hcolor==6 && hold[a-1][j]) || (block.hcolor!=6 && hold[a][j])){
                if(block.hcolor == 0)
                    tomato
                else if(block.hcolor == 1)
                    yellow
                else if(block.hcolor == 2)
                    lightblue
                else if(block.hcolor == 3)
                    pink
                else if(block.hcolor == 4)
                    green
                else if(block.hcolor == 5)
                    orange
                else if(block.hcolor == 6)
                    blue
            }
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
        else if(a==4 || a==9)
            printf("----------");
        else if(a==5)
            printf("-score----");
        else if(a<9){
            printf("|");
            if(a<4)
                holdhyoji(a);
            else if(a == 6 || a == 8)
                printf("        ");
            else if(a == 7)
                printf(" %6d ",score);
            printf("|");
        }

        puts("");
    }
    fflush(stdout);
    return 0;
}

void mainclear(void) {
    //field[0]配列を0にリセットしておく
    int i, j;
    for(i=0; i<20; i++)
        for(j=0; j<12; j++)
            field[0][i][j] = 0;
    /*nextpush関数でnext[][][1]に落ちてくるやつを設定しているので
    それを実際に落ちてくる配列block.blockと隠し配列next[][][0]にセットする*/
    for(i=0; i<4; i++)
        for(j=0; j<4; j++){
            next[i][j][0] = block.block[i][j] = next[i][j][1];
            hold[i][j] = 0;
        }
}