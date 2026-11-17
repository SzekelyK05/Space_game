#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <string>
#include <ctime>
#include "game.h"

using namespace std;

const int width = 80;
const int height = 20;

int posShipX, posShipY;
int shotCordX, shotCordY;
int enemyCordX[5], enemyCordY[5];
bool shotVisible;
int score;
bool isGameOver;
int enemySpeed;

void hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void gameInit()
{
	srand(time(0));

	isGameOver = false;
	posShipX = 10;
	posShipY = height / 2;
	shotCordX = 11;
	shotCordY = height / 2;
	score = 0;
	shotVisible = true;
	enemySpeed = 0;
	for (int i = 0; i < 5; i++)
	{
		enemyCordX[i] = width + (i * 15) + (rand() % 15);
		enemyCordY[i] = rand() % height;
	}

	hideCursor();
}

void resetCursor() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0, 0 };
	SetConsoleCursorPosition(hOut, coord);
}

void gameRender()
{
	resetCursor();

	string frame = "";

	for (int i = 0; i < width + 2; i++)
		frame += "-";
	frame += "\n";


    for (int i = 0; i < height; i++) {
        for (int j = 0; j <= width; j++) {

			bool printed = false;

            if (j == 0 || j == width)
			{
				frame += "|";
				printed = true;
			}
			else if (i == posShipY && j == posShipX)
			{
				frame += "H";
				printed = true;
			}
			else if (shotVisible && i == shotCordY && j == shotCordX)
			{
				frame += "-";
				printed = true;
			}
			else
			{
				for (int enemy = 0; enemy < 5; enemy++)
				{
					if (i == enemyCordY[enemy] && j == enemyCordX[enemy])
					{
						frame += "O";
						printed = true;
						break;
					}
				}
			}

			if (!printed)
				frame += " ";
            
        }
		frame += "\n";
    }

	for (int i = 0; i < width + 2; i++)
		frame += "-";
	frame += "\n";

	frame += "Score: " + to_string(score) + "\n";

	cout << frame;

}

void updateGame()
{

	if (shotCordX >= width || !shotVisible && shotCordX >= width - 15)
	{
		shotVisible = true;
		shotCordX = posShipX + 1;
		shotCordY = posShipY;
	}
	else
		shotCordX++;

	enemySpeed++;

	for (int enemy = 0; enemy < 5; enemy++)
	{
		if (enemySpeed >= 4)
		{
			enemyCordX[enemy]--;
		}
		
		if (shotVisible && shotCordY == enemyCordY[enemy] && (shotCordX == enemyCordX[enemy] || shotCordX == enemyCordX[enemy] + 1))
		{
			shotVisible = false;
			score++;
			enemyCordX[enemy] = width + (enemy * 15) + (rand() % 15);
			enemyCordY[enemy] = rand() % height;
		}
		else if (enemyCordX[enemy] == 0)
			isGameOver = true;
	}	
	
	if (enemySpeed >= 4)
	{
		enemySpeed = 0;
	}

}

void userInput()
{
	if (GetAsyncKeyState('A') & 0x8000) {
		if (posShipX > 1) posShipX--;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		if (posShipX < width - 1) posShipX++;
	}
	if (GetAsyncKeyState('W') & 0x8000) {
		if (posShipY > 0) posShipY--;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		if (posShipY < height - 1) posShipY++;
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		isGameOver = true;
	}
}