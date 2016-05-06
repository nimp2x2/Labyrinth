#ifndef MENU_H
#define MENU_H

#include "EngineHeaders.h"

class Menu
{
	ISprite *mainMenuLogo;
	DWORD mainMenuSelection;
	DWORD currentState;
	float logoy;
	bool fullscreen;
	int currentMode;
public:
	void AdjustForWidthAndHeight();
	static bool Render(Menu *menu);
	static DWORD Logic(Menu *menu);
	Menu();
	~Menu();
};

#endif