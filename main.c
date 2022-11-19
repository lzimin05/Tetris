#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "levels.h"

#define N 60//L
#define M 18 //H
#define Z 58 // board
#define S 3000 //ms Sleep
#define Slow 1

int Score = 0;
char GameOver = false;

void setcur(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

char** organizationPole(int n, int m) {
    char** a = (char**) malloc(sizeof(char*)*m);
    for(int i = 0; i < m; i++) {
        a[i] = (char*) malloc(sizeof(char)*n);
    }
    return a;
}

struct Player {
    int* x;
    int size;
};

struct Ball {
    float x; //N
    float y; //M
    float speedX;
    float speedY;
};

struct Player organizationPlayer(int n, int m, int z) {
    struct Player pl;
    pl.x = (int*) malloc(sizeof(int)*z);
    pl.size = z;
    int k = (pl.size/2)*-1;
    for(int i = 0; i < z; i++){
        pl.x[i] = (n/2)+k;
        k++;
    }
    return pl;
}

char** BuildPole(char** pole, int n, int m, struct Player pl, struct Ball ball, struct Blocks* blocks) {
    for(int i = 0; i < m; i++) {
        for(int g = 0; g < n; g++){
            if(i == 0 || g == 0 || g == N-1) {
                pole[i][g] = '#';
            } else if(i == M-1) {
                pole[i][g] = '-';
            } else {
                pole[i][g] = ' ';
            }
            if(i == m-1) {
                for(int z = 0; z < pl.size; z++) {
                    pole[i][pl.x[z]] = '=';
                }

            }
            if(round(ball.y) == i && round(ball.x) == g) {
                pole[i][g] = '*';
            }
        }
    }
    for(int i = 0; i < level.sizeBlocks; i++) {
        if(blocks[i].stay) {
            pole[blocks[i].y][blocks[i].x] = '#';
        }
    }
    return pole;
}

void PrintPole(char** pole, int n, int m) {
    for(int i = 0; i < m; i++){
        for(int g = 0; g < n; g++){
            printf("%c", pole[i][g]);
        }
        printf("\n");
    }
}

struct Player movePlayer(char move, struct Player pl, int n) {
    if(move == 'a' && pl.x[0] > 1) {
        for(int z = 0; z < pl.size; z++) {
            pl.x[z] -= 1;
        }
    }
    if(move == 'd' && pl.x[pl.size-1] < n-2) {
        for(int z = 0; z < pl.size; z++) {
            pl.x[z] += 1;
        }
    }
    return pl;
}

struct Ball moveWithBoard(struct Player pl, int m) {
    struct Ball ball;
    ball.x = pl.x[pl.size/2];
    ball.y = m-2;
    ball.speedX = 0;
    ball.speedY = 0;
    return ball;
}

struct Ball physicsBall(struct Ball ball, struct Player pl, struct Blocks* blocks, char** pole,int n, int m) {
    if(ball.speedY > 0 && round(ball.y) == m-2) {
        int i = -1;
        while(i < pl.size) {
            i++;
            if(round(ball.x) == pl.x[i]) {
                i = i * 100;
            }
        }
        if(i < 100) {
            EndGames("GAME OVER");
            //íàäî äîïèñàòü åñëè óïàë ìÿ÷
        } else {
            i/=100;
            int x = i-(pl.size/2+1);
            ball.speedX = cos(x/(x*x+1));           //óãîë íàêëîíà
            ball.speedY = sin(1/(x*x+1));
            if(x < 0) {
                ball.speedX = ball.speedX*(-1);
            }
            if(ball.speedY < 0.1) {
                ball.speedY = 0.15;
            }
            ball.speedY = ball.speedY*(-1);
        }
    }
    /*
    if(ball.speedY < 0 && round(ball.y) == 1) {
        ball.speedY = ball.speedY*(-1);
    }
    if(round(ball.x) == 1 || round(ball.x) == n-2) {
        ball.speedX = ball.speedX*(-1);
    }
    */

    if(pole[(int)round(ball.y+1)][(int)round(ball.x)] == '#') {
        ball.speedY = ball.speedY*(-1);
        if((int)round(ball.y+1) != 0) {
            for(int i = 0; i < level.sizeBlocks; i++) {
                if(blocks[i].x == (int)round(ball.x) && blocks[i].y == (int)round(ball.y+1)) {
                    blocks[i].stay = false;
                    Score+=50;
                }
            }
        }
    } else if(pole[(int)round(ball.y-1)][(int)round(ball.x)] == '#') {
        ball.speedY = ball.speedY*(-1);
        for(int i = 0; i < level.sizeBlocks; i++) {
            if(blocks[i].x == (int)round(ball.x) && blocks[i].y == (int)round(ball.y-1)) {
                blocks[i].stay = false;
                Score+=50;
            }
        }
    }
    if(pole[(int)round(ball.y)][(int)round(ball.x+1)] == '#') {
        ball.speedX = ball.speedX*(-1);
        if((int)round(ball.x+1) != N-1) {
            for(int i = 0; i < level.sizeBlocks; i++) {
                if(blocks[i].x == (int)round(ball.x+1) && blocks[i].y == (int)round(ball.y)) {
                    blocks[i].stay = false;
                    Score+=50;
                }
            }
        }
    } else if(pole[(int)round(ball.y)][(int)round(ball.x-1)] == '#') {
        ball.speedX = ball.speedX*(-1);
        if((int)round(ball.x-1) != 0) {
            for(int i = 0; i < level.sizeBlocks; i++) {
                if(blocks[i].x == (int)round(ball.x-1) && blocks[i].y == (int)round(ball.y)) {
                    blocks[i].stay = false;
                    Score+=50;
                }
            }
        }
    }

    ball.x+=ball.speedX/Slow;
    ball.y+=ball.speedY/Slow;
    return ball;
}

bool EndLevel(struct Blocks* blocks) {
    for(int i = 0; i < level.sizeBlocks; i++) {
        if(blocks[i].stay == true) {
            return false;
        }
    }
    system("cls");
    printf("\n\n\n\t\t\tNext Level %i", level.level);
    return true;
}

void EndGames(char* text) {
    system("cls");
    printf("\n\n\t\t\t%s\n\t\t\tYour Score %i",text, Score);
    GameOver = true;
    getch();
}

int main() {
    char** pole = organizationPole(N, M);
    struct Player player= organizationPlayer(N, M, Z);
    struct Ball ball;
    short int proverka = 0;
    level.level = 1;
    struct Blocks* blocks = Level_1(N, M);

    while(GameOver != true) {
        if(level.level == -1) {
            EndGames("You are a winner");
            sleep(100);
            return 100;

        }
        pole = BuildPole(pole, N, M, player, ball, blocks);
        PrintPole(pole, N, M);

        if(GetKeyState('A') < 0) {
            player = movePlayer('a', player, N);

        }
        if(GetKeyState('D') < 0) {
            player = movePlayer('d', player, N);

        }
        if(GetKeyState(' ') < 0 && proverka == 0) {
            ball.speedX = 1;
            ball.speedY = -1;
            proverka = 1;
        }

        if(proverka == 0) {
            ball = moveWithBoard(player, M);
        }

        if(proverka == 1) {
            ball = physicsBall(ball, player, blocks, pole, N, M);
        }
        if(EndLevel(blocks)) {
            //÷òî äåëàòü äàëüøå?
            Score += 1000;
            pole = organizationPole(N, M);
            player= organizationPlayer(N, M, Z);
            proverka = 0;
            level.level++;
            free(blocks);
            blocks = Level_1(N, M);
        }
        usleep(S); //ìèëëèñåêóíäû
        setcur(0, 0);
    }
    sleep(S);
    return 0;
}//Level_1
