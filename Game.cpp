#include "Game.h"

/*
SceneId:
2 - menuinit
3 - menu
4 - builderinit
5 - builder
6 - maingameinit
7 - maingame
*/

Game game;

Game::Game()
{
	this->currentGameScene = 2;
	this->menu = NULL;
	this->builder = NULL;
	this->mgame = NULL;
}

Game::~Game()
{
	SAFE_RELEASE(this->menu);
	SAFE_RELEASE(this->builder);
	SAFE_RELEASE(this->mgame);
}

bool Game::Logic()
{
	switch(game.currentGameScene)
	{
		//menu
	case 2:
		if(game.menu == NULL)
		{
			game.menu = new Menu();
			game.menu->AdjustForWidthAndHeight();
			engineHeaders.OnLost();
			engineHeaders.window->SetFullscreenState(!engineHeaders.window->GetFullscreenState());
			if(engineHeaders.window->GetFullscreenState())
			{
				engineHeaders.device->GetDevice()->Reset(&engineHeaders.device->d3dPPFullScreen);

			}else{
				engineHeaders.device->GetDevice()->Reset(&engineHeaders.device->d3dPPWindowed);
			}
			engineHeaders.OnReset();
		}
		game.result = 3;
		SQUALL_Channel_Pause(engineHeaders.channel1, 0);
		SQUALL_Channel_SetPlayPosition(engineHeaders.channel1, 0);
		SQUALL_Channel_Pause(engineHeaders.channel2, 1);
		break;
	case 3:
		game.result = game.menu->Logic(game.menu);
		break;
	case 4:
		if(game.builder == NULL)
			game.builder = new Builder();
		game.result = 5;
		break;
	case 5:
		game.result = game.builder->Logic(game.builder);
		break;
	case 6:
		if(game.mgame == NULL)
			game.mgame = new MainGame;
		if(game.mgame->filename != NULL)
			delete []game.mgame->filename;
		game.mgame->filename = new wchar_t[256];
		game.mgame->filename[0] = 0;
		game.mgame->filenamepos = 0;
		game.result = 7;
		break;
	case 7:
		game.result = game.mgame->Logic(game.mgame);
		break;
	}
	if(game.result == 7)
	{
		SQUALL_Channel_Pause(engineHeaders.channel1, 1);
		SQUALL_Channel_Pause(engineHeaders.channel2, 0);
		SQUALL_Channel_SetPlayPosition(engineHeaders.channel2, 0);
	}

	engineHeaders.input->ClearInput();
	if(game.result == 1)
		return true;
	else if(game.result == 0)
		return false;
	else
	{
		game.currentGameScene = game.result;
		return true;
	}
}

bool Game::Render()
{
	HRESULT hr = engineHeaders.device->GetDevice()->TestCooperativeLevel();
	if(hr == D3DERR_DRIVERINTERNALERROR)
		return false;
	if(hr == D3DERR_DEVICELOST || hr == D3DERR_DEVICENOTRESET)
	{
		engineHeaders.OnLost();
		if(hr != D3DERR_DEVICENOTRESET)
		{
			Sleep(750);
			return true;
		}
		
		if(engineHeaders.window->GetFullscreenState())
		{
			engineHeaders.device->GetDevice()->Reset(&engineHeaders.device->d3dPPFullScreen);

		}else{
		engineHeaders.device->GetDevice()->Reset(&engineHeaders.device->d3dPPWindowed);
		}
		engineHeaders.OnReset();
	}

	engineHeaders.device->GetDevice()->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	engineHeaders.device->GetDevice()->BeginScene();
	switch(game.currentGameScene)
	{
	case 3:
		if(!game.menu->Render(game.menu))
		{
			engineHeaders.device->GetDevice()->EndScene();
			engineHeaders.device->GetDevice()->Present(NULL, NULL, NULL, NULL);
			return false;
		}
		break;
	case 5:
		if(!game.builder->Render(game.builder))
		{
			engineHeaders.device->GetDevice()->EndScene();
			engineHeaders.device->GetDevice()->Present(NULL, NULL, NULL, NULL);
			return false;
		}
		break;
	case 7:
		if(!game.mgame->Render(game.mgame))
		{
			engineHeaders.device->GetDevice()->EndScene();
			engineHeaders.device->GetDevice()->Present(NULL, NULL, NULL, NULL);
			return false;
		}
		break;
	}
	engineHeaders.device->GetDevice()->EndScene();
	engineHeaders.device->GetDevice()->Present(NULL, NULL, NULL, NULL);
	return true;
}