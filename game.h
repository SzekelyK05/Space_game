#ifndef HEADER_H
#define HEADER_H


extern const int width;
extern const int height;

extern int posShipX, posShipY;
extern int enemyCordX[5], enemyCordY[5];
extern int fireCooldown;

extern int score;
extern bool isGameOver;
extern int enemySpeed;

void gameInit();

void gameRender();

void updateGame();

void userInput();

#endif