#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <string>
#include <ctime>
#include "game.h"
using namespace std;


int main()
{
	gameInit();
	
	while (!isGameOver)
	{
		gameRender();
		userInput();
		updateGame();
		Sleep(30);
	}

	return 0;
}