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
int score;
bool isGameOver;

int fireCooldown;


shotClass shot[3];

void shotClass::initShot() {
	shotCordX = 0;
	shotCordY = 0;
	shotVisible = false;
}

void shotClass::fire(int startX, int startY) {
	shotCordX = startX;
	shotCordY = startY;
	shotVisible = true;
}

void shotClass::moveShot() {
	shotCordX++;
	if (shotCordX >= width)
		shotVisible = false;
}

enemyClass enemy[5];

void enemyClass::initEnemy(int i) {
	enemySpeed = 0;
	enemyCordX = width + (i * 15) + (rand() % 15);
	enemyCordY = rand() % height;

	step = rand() % 4 + 1;
}


void enemyClass::moveEnemy(int i) {
	enemyCordX = width + (i * 15) + (rand() % 15);
	enemyCordY = rand() % height;
}

void enemyClass::handleSpeed() {
	

	if (enemySpeed >= step)
	{
		enemyCordX--;
		enemySpeed = 0;
	}
	else
		enemySpeed++;
}

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
	
	for (int i = 0; i < 5; i++)
	{
		enemy[i].initEnemy(i);
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
				for (int enemyInd = 0; enemyInd < 5; enemyInd++)
				{
					if (i == enemy[enemyInd].enemyCordY && j == enemy[enemyInd].enemyCordX)
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


	for (int enemyInd = 0; enemyInd < 5; enemyInd++)
	{

		enemy[enemyInd].handleSpeed();

		for (int s = 0; s < 3; s++)
		{
			if (shot[s].shotVisible && shot[s].shotCordY == enemy[enemyInd].enemyCordY &&
				(shot[s].shotCordX == enemy[enemyInd].enemyCordX || shot[s].shotCordX == enemy[enemyInd].enemyCordX + 1))
			{
				shot[s].shotVisible = false;
				score++;
				enemy[enemyInd].moveEnemy(enemyInd);
			}
		}

		if (enemy[enemyInd].enemyCordX == 0)
			isGameOver = true;
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