#ifndef LABBUILDER_H
#define LABBUILDER_H

#include "EngineHeaders.h"

class Builder
{
	UINT **map;
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
	DWORD builderState; // 0 - preparation, 1 - choose width & height, 2 - build your labyrinth, 3 - new or open file? , 4 - enter filename, 5 - file not exist, 6 choose filename
	wchar_t *filename;
	UINT filenamepos;
	ISprite *emptyBlock;
	ISprite *wall;
	ISprite *startPoint;
	ISprite *finishPoint;
	bool widthChosen;
	wchar_t *chooseString;
	DWORD stringPos;
public:
	static bool Render(Builder *builder);
	static DWORD Logic(Builder *builder);
	Builder();
	~Builder();
};
#endif