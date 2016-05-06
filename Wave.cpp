#include "wave.h"

stack* FindPath(unsigned int startX, unsigned int startY, unsigned int targetX, unsigned int targetY, unsigned int **map, unsigned int n, unsigned int m)
{
	//0 - empty, 1 - wall, 2 - finish, 3 start
	UINT currentStep = 3;
	bool founded = true;
	bool wayExist = false;
	map[targetX][targetY] = 2;
	map[startX][startY] = 3;

	for(UINT i = 0; i < n; i++)
		for(UINT j = 0; j < m; j++)
			if(map[i][j] != 1 && map[i][j] != 2 && map[i][j] != 3)
				map[i][j] = 0;

	while(founded)
	{
		founded = false;
		for(UINT i = 0; i < n; i++)
			for(UINT j = 0; j < m; j++)
			{
				if(map[i][j] == currentStep)
				{
					if((i+1) < (n))
					{
						if(map[i+1][j] == 2)
						{
							map[i+1][j] = currentStep+1;
							i = n;
							founded = false;
							wayExist = true;
							break;
						}
						if(map[i+1][j] == 0)
						{
							map[i+1][j] = currentStep+1;
							founded = true;
						}
					}
					if(i != 0)
					{
						if(map[i-1][j]  == 2)
						{
							map[i-1][j] = currentStep+1;
							i = n;
							founded = false;
							wayExist = true;
							break;
						}
						if(map[i-1][j] == 0 )
						{
							map[i-1][j] = currentStep+1;
							founded = true;
						}
					}
					if((j+1) < (m))
					{
						if(map[i][j+1] == 2)
						{
							map[i][j+1]  = currentStep+1;
							i = n;
							founded = false;
							wayExist = true;
							break;
						}
						if(map[i][j+1]  == 0 )
						{
							map[i][j+1]  = currentStep+1;
							founded = true;
						}
					}
					if(j != 0)
					{
						if(map[i][j-1] == 2)
						{
							map[i][j-1] = currentStep+1;
							i = n;
							founded = false;
							wayExist = true;
							break;
						}
						if(map[i][j-1] == 0)
						{
							map[i][j-1] = currentStep+1;
							founded = true;
						}
					}
				}
			}
			currentStep++;
	}
	if(!wayExist)
		return NULL;

	UINT topVal, botVal, leftVal, rightVal;
	UINT sortArray[4];
	UINT tempVal;
	stack *temp = new stack();
	stack *top = NULL;

	while(currentStep != 3)
	{
		leftVal = 4294967295;
		botVal = 4294967295;
		rightVal = 4294967295;
		topVal = 4294967295;
		if((targetX) != 0)
			if(map[targetX-1][targetY] != 0 && map[targetX-1][targetY] != 1)
				leftVal = map[targetX-1][targetY];
			
		if((targetX+1) < n)
			if(map[targetX+1][targetY] != 0 && map[targetX+1][targetY] != 1)
				rightVal = map[targetX+1][targetY];
			
		if((targetY) != 0)
			if(map[targetX][targetY-1] != 0 && map[targetX][targetY-1] != 1)
				botVal = map[targetX][targetY-1];
			
		if((targetY+1) < m)
			if(map[targetX][targetY+1] != 0 && map[targetX][targetY+1] != 1)
				topVal = map[targetX][targetY+1];

		sortArray[0] = leftVal; 
		sortArray[1] = rightVal;
		sortArray[2] = botVal; 
		sortArray[3] = topVal;
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 3; j++)
				if(sortArray[j] > sortArray[j+1])
				{
					tempVal = sortArray[j];
					sortArray[j] = sortArray[j+1];
					sortArray[j+1] = tempVal;
				}
		if(sortArray[0] == leftVal)
		{
			targetX -= 1;
		}else if(sortArray[0] == rightVal)
		{
			targetX += 1;
		}else if(sortArray[0] == botVal)
		{
			targetY -= 1;
		}else
		{
			targetY += 1;
		}
		currentStep--;
		if(currentStep != 3)
			temp->Push(top,targetX,targetY);
	}
	delete temp;
	return top;
}

stack::stack()
{
	this->x = this->y = 0;
	this->prev = 0;
}

stack::~stack()
{
}

void stack::Push(stack *&top,UINT x, UINT y)
{
	if(top == NULL)
	{
		top = new stack();
		top->x = x;
		top->y = y;
	}else
	{
		stack *temp = new stack();
		temp->x = x;
		temp->y = y;
		temp->prev = top;
		top = temp;
	}
}

void stack::Pop(stack *&top, UINT &x, UINT &y)
{
	x = y = 0;
	if(top != NULL)
	{
		x = top->x;
		y = top->y;
		stack *temp = top;
		top = top->prev;
		delete temp;
	}
}
