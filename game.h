#ifndef HEADER_H
#define HEADER_H


extern const int width;
extern const int height;

extern int posShipX, posShipY;

extern int fireCooldown;

extern int score;
extern bool isGameOver;

class shotClass {
public:
	int shotCordX, shotCordY;
	bool shotVisible;

	void initShot();

	void fire(int startX, int startY);

	void moveShot();
};

class enemyClass {
	public:
		int enemyCordX, enemyCordY, enemySpeed, step;

		void initEnemy(int i);

		void moveEnemy(int i);

		void handleSpeed();
};

void gameInit();

void gameRender();

void updateGame();

void userInput();

#endif