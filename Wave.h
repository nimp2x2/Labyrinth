#ifndef WAVE_H
#define WAVE_H

#include "EngineHeaders.h"

class stack
{
public:
	UINT x;
	UINT y;
	stack *prev;
	void Pop(stack *&top, UINT &x, UINT &y);
	void Push(stack *&top, UINT x, UINT y);
	stack();
	~stack();
};

stack* FindPath(unsigned int startX, unsigned int startY, unsigned int targetX, unsigned int targetY, unsigned int **map, unsigned int n, unsigned int m);

#endif