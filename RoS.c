#include "RoS.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h> 
#include <time.h>
#include <errno.h>
#include <wchar.h>
#include <locale.h>

#define gridSizeX 8
#define gridSizeY 8
#define skipTitle 0

int playerX = 2;
int playerY = 2;
int roomX = 1;
int roomY = 1;


char inputStr[50];
wchar_t gameScreen[gridSizeY][gridSizeX];
wchar_t collisionScreen[gridSizeY][gridSizeX];
// Wait milliseconds.
int msleep(long msec) {
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
// Clear -> Print -> Sleep.
void clrPntSlp(wchar_t str[256], long s) {
    system("clear");
    wprintf(str);
    fflush(stdout);
    msleep(s);}
// Clear -> Print.
void clrPnt(wchar_t str[256]) {
    system("clear");
    wprintf(str);}
// Print -> Sleep.
void pntSlp(wchar_t str[256], long s) {
    wprintf(str);
    fflush(stdout);
    msleep(s);}
// Clear -> Sleep.
void clrSlp(long s) {
    system("clear");
    fflush(stdout);
    msleep(s);}
SmallRoom getSmallRoom(char str[128]) {
    FILE * fp;
    fp = fopen(str,"r");
    if (fp == NULL) {
        printf("Null room file.");
    }

    SmallRoom room;

    int i,j;

    for(i=0; i<16; i++) {
        if (i<8) {
            for(j=0; j<8; j++) {
                fwscanf(fp,L" %lc",&room.roomArr[i][j]);
            }
        }
        else {
            for(j=0; j<8; j++) {
                fwscanf(fp,L" %lc",&room.collisionArr[i-8][j]);
            }
        }
    }
    return room;
}
void drawScr() {
    for (int i = 0; i < gridSizeY; i++) {
        for (int j = 0; j < gridSizeX; j++) {
            wprintf(L"%lc ", gameScreen[i][j]);
        }
        wprintf(L"\n");
    }
}
void clearScr() {
    for (int i = 0; i < gridSizeY; i++) {
        for (int j = 0; j < gridSizeX; j++) {
            gameScreen[i][j] = L'\0';
        }
        for (int j = 0; j < gridSizeX; j++) {
            collisionScreen[i][j] = L'\0';
        }
    }
}
void blitObj(wchar_t ch, int x, int y) {
    gameScreen[x][y] = ch;
}
void blitSmallRoom(SmallRoom room) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            gameScreen[j][i] = room.roomArr[j][i];
        }
        for (int j = 0; j < 8; j++) {
            collisionScreen[j][i] = room.collisionArr[j][i];
        }
    }
}
void fullDraw() {
    char roomBuffer[128];
    snprintf(roomBuffer, sizeof(roomBuffer), "Room(%d,%d).txt", roomX, roomY);
    blitSmallRoom(getSmallRoom(roomBuffer));
    blitObj(L'@', playerY, playerX);
    drawScr();
}
void getInput() {
    while(1) {
        for (int i = 0; i < strlen(inputStr); i++) {
            inputStr[i] = L'\0';
        }
        scanf("%s",inputStr);
        if(!strcmp(inputStr, "a") || !strcmp(inputStr, "left") || !strcmp(inputStr, "Left ")) {
            if (playerX - 1 < 0) {
                playerX = gridSizeX-1;
                roomX -= 1;
                break;
            }
            if (collisionScreen[playerY][playerX - 1] == '.') {   
                playerX -= 1;
                break;
            }
            else {
                system("clear");
                fullDraw();
            }
        }
        else if(!strcmp(inputStr, "w") || !strcmp(inputStr, "up") || !strcmp(inputStr, "Up ")) {
            if (playerY - 1 < 0) {
                playerY = gridSizeY-1;
                roomY -= 1;
                break;
            }
            if (collisionScreen[playerY - 1][playerX] == '.') {
                    playerY -= 1;
                break;
            }
            else {
                system("clear");
                fullDraw();
            }
        }
        else if(!strcmp(inputStr, "d") || !strcmp(inputStr, "right") || !strcmp(inputStr, "Right ")) {
            if (playerX + 1 > gridSizeX-1) {
                playerX = 0;
                roomX += 1;
                break;
            }
            if (collisionScreen[playerY][playerX + 1] == '.') {             
                playerX += 1;
                break;
            }
            else {
                system("clear");
                fullDraw();
            }
        }
        else if(!strcmp(inputStr, "s") || !strcmp(inputStr, "down") || !strcmp(inputStr, "Down ")) {
            if (playerY + 1 > gridSizeY-1) {
                playerY = 0;
                roomY += 1;
                break;
            }
            if (collisionScreen[playerY + 1][playerX] == '.') {             
                playerY += 1;
                break;
            }
            else {
                system("clear");
                fullDraw();
            }
        }
        else {
            wprintf(L"Invalid Input\n");
            fflush(stdout);
            msleep(500);
            system("clear");
            fullDraw();
        }
    }
}
void gameLoop() {
    while (1) {
        system("clear");
        fullDraw();
        getInput();
        clearScr();
    }
}
void titleScr() {
    fflush(stdout);
    clrPntSlp(L"\n-=[><>               ]=-\n\n", 500);
    clrPntSlp(L"\n-=[><><><><          ]=-\n\n", 500);
    clrPntSlp(L"\n-=[><><><><><><>     ]=-\n\n", 500);
    clrPntSlp(L"\n-=[><><><><><><><><><]=-\n\n", 500);

    while(1)
    {
        clrPnt(L"\n-=[Return of Sambooga]=-\n\n");
        wprintf(L"Type *Start* to start.\n");

        for (int i = 0; i < strlen(inputStr); i++) {
            inputStr[i] = L'\0';
        }
        scanf("%s",inputStr);
        if(!strcmp(inputStr, "start") || !strcmp(inputStr, "Start")) {
            gameLoop();
            break;
        }
        else if (!strcmp(inputStr, "exit") || !strcmp(inputStr, "Exit")) {
            wprintf(L"Bye Bye\n");
            sleep(1);
            system("clear");
            exit(0);
        }
        else if (!strcmp(inputStr, "Petzel64") || !strcmp(inputStr, "Petzel")){
            pntSlp(L"Hacking the Mainframe...\n", 2000);
            clrPntSlp(L"Hacking failed. =(\n", 1000);
        }
        else {
            wprintf(L"Invalid Input\n");
            fflush(stdout);
            msleep(500);
        }
    }
}
int main() {
    setlocale(LC_CTYPE, "");
    if (skipTitle == 0) {
        titleScr();
    }
    else {
        gameLoop();
    }
    return 0;
}