#ifndef GAME_H
#define GAME_H

#include "Menu.h"
#include "LabBuilder.h"
#include "MainGame.h"

class Game
{
	Menu *menu;
	Builder *builder;
	MainGame *mgame;
	DWORD currentGameScene;
	DWORD result;
public:
	static bool Render();
	static bool Logic();
	Game();
	~Game();
};

extern Game game;

#endif