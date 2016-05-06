#ifndef MAINGAME_H
#define MAINGAME_H

#include "Wave.h"

class MainGame
{
	bool pathFinded;
	bool showPath;
	bool pathExist;
	DWORD mainGameState;
	UINT **map;
	bool **map2;
	UINT mapWidth;
	UINT mapHeight;
	UINT currentPosX;
	UINT currentPosY;
	int offsetx;
	int offsety;
	int xmax;
	int ymax;
	int xadd;
	int yadd;
	int icounter;
	int jcounter;
	int dx;
	int dy;
	UINT oldx;
	UINT oldy;
	bool drag;
	UINT startX;
	UINT startY;
	std::vector<UINT> dest;
	ISprite *emptyBlock;
	ISprite *wall;
	ISprite *startPoint;
	ISprite *finishPoint;
	ISprite *upArrow;
	ISprite *downArrow;
	ISprite *leftArrow;
	ISprite *rightArrow;
	ISprite *undiscovered;
	DWORD currentExit;
	//3d stuff
	bool mouseClip;
	int camx;
	int camOldx;
	int camy;
	int camOldy;
	bool recover;
	void PerformCollisionDetection();
public:
	wchar_t *filename;
	UINT filenamepos;
	static DWORD Logic(MainGame *mgame);
	static bool Render(MainGame *mgame);
	MainGame();
	~MainGame();
};

#endif