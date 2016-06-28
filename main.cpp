#include "Game.h"

INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if(engineHeaders.Init(hInst))
	{
		engineHeaders.window->LogicFunction = game.Logic;
		engineHeaders.window->RenderFunction = game.Render;
		engineHeaders.Run();
	}
	return 0;
}
