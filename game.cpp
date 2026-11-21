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
int enemyCordX[5], enemyCordY[5];
int score;
bool isGameOver;
int enemySpeed;

int fireCooldown;

class shotClass {
	public:
		int shotCordX, shotCordY;
		bool shotVisible;

		void initShot()
		{
			shotCordX = 0;
			shotCordY = 0;
			shotVisible = false;
		}

		void fire(int startX, int startY)
		{
			shotCordX = startX;
			shotCordY = startY;
			shotVisible = true;
		}

		void moveShot()
		{
			shotCordX++;
			if (shotCordX >= width)
				shotVisible = false;
		}
};

shotClass shot[3];

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
	
	for(int i=0; i<3; i++)
		shot[i].initShot();
	isGameOver = false;
	posShipX = 10;
	posShipY = height / 2;
	
	score = 0;
	
	enemySpeed = 0;
	for (int i = 0; i < 5; i++)
	{
		enemyCordX[i] = width + (i * 15) + (rand() % 15);
		enemyCordY[i] = rand() % height;
	}

	fireCooldown = 0;

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
			else if ((shot[0].shotVisible && i == shot[0].shotCordY && j == shot[0].shotCordX) ||
				(shot[1].shotVisible && i == shot[1].shotCordY && j == shot[1].shotCordX) ||
				(shot[2].shotVisible && i == shot[2].shotCordY && j == shot[2].shotCordX))
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
	

	for(int i=0; i<3; i++)
	{
		if (shot[i].shotVisible)
			shot[i].moveShot();
	}

	enemySpeed++;

	for (int enemy = 0; enemy < 5; enemy++)
	{
		if (enemySpeed >= 4)
			enemyCordX[enemy]--;

		for (int s = 0; s < 3; s++)
		{
			if (shot[s].shotVisible && shot[s].shotCordY == enemyCordY[enemy] &&
				(shot[s].shotCordX == enemyCordX[enemy] || shot[s].shotCordX == enemyCordX[enemy] + 1))
			{
				shot[s].shotVisible = false;
				score++;
				enemyCordX[enemy] = width + (enemy * 15) + (rand() % 15);
				enemyCordY[enemy] = rand() % height;
			}
		}

		if (enemyCordX[enemy] == 0)
			isGameOver = true;
	}
	
	if (enemySpeed >= 4)
	{
		enemySpeed = 0;
	}

}

void userInput()
{
	if (fireCooldown > 0) fireCooldown--;

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
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && fireCooldown == 0) {
		for (int i = 0; i < 3; i++) {
			if (!shot[i].shotVisible) {
				shot[i].fire(posShipX + 1, posShipY);
				fireCooldown = 5;
				break;
			}
		}
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		isGameOver = true;
	}
}