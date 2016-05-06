#include "Menu.h"

/*
MenuStates
0 - main screen
1 - help scrren
2 - options screen
3 - labirynth screen
*/

Menu::Menu()
{
	this->mainMenuLogo = new ISprite(924.0f, 81.0f, 2.0f, D3DCOLOR_XRGB(255,255,255), engineHeaders.resourceManager->GetTexture(L"Data/Textures/mainMenuLogo.dds"));
	this->mainMenuSelection = 1;
	this->currentState = 0;
	this->logoy = 0.85f;
	this->currentMode = engineHeaders.device->modesForEachFormat[0]-1;
	this->fullscreen = engineHeaders.window->GetFullscreenState();
}

Menu::~Menu()
{
	SAFE_RELEASE(this->mainMenuLogo);
}

DWORD Menu::Logic(Menu *menu)
{
	switch(menu->currentState)
	{
	case 0:
		if(engineHeaders.input->GetKeyPressNumber(VK_ESCAPE) > 0)
			return 0;
		if(engineHeaders.input->GetKeyPressNumber(VK_LBUTTON))
		{
			if(engineHeaders.input->GetMouseX() > engineHeaders.currentWidth/2-120 && engineHeaders.input->GetMouseX() < engineHeaders.currentWidth/2+110
					&& engineHeaders.input->GetMouseY() > (engineHeaders.currentHeight/2) && engineHeaders.input->GetMouseY() < (engineHeaders.currentHeight/2+engineHeaders.font->GetScale()*32.0f))
			{
				menu->mainMenuSelection = 0;
				return 6;
			}

			if(engineHeaders.input->GetMouseX() > engineHeaders.currentWidth/2-220 && engineHeaders.input->GetMouseX() < engineHeaders.currentWidth/2+210
					&& engineHeaders.input->GetMouseY() > (engineHeaders.currentHeight/2-engineHeaders.font->GetScale()*32.0f) && engineHeaders.input->GetMouseY() < (engineHeaders.currentHeight/2))
			{
				menu->mainMenuSelection = 1;
				return 4;
			}

			if(engineHeaders.input->GetMouseX() > engineHeaders.currentWidth/2-70 && engineHeaders.input->GetMouseX() < engineHeaders.currentWidth/2+70
					&& engineHeaders.input->GetMouseY() > (engineHeaders.currentHeight/2-engineHeaders.font->GetScale()*32.0f*2) && engineHeaders.input->GetMouseY() < (engineHeaders.currentHeight/2-engineHeaders.font->GetScale()*32.0f))
			{
				menu->mainMenuSelection = 2;
				menu->currentState = 2;
				menu->fullscreen = engineHeaders.window->GetFullscreenState();
				menu->currentMode = engineHeaders.device->modesForEachFormat[0]-1;
				for(int i = 0; i < engineHeaders.device->modesForEachFormat[0]; i++)
				{
					if(engineHeaders.device->displayModes[0][i].Width == engineHeaders.currentWidth)
					{
						menu->currentMode = i;
						break;
					}
				}
				
			}
			if(engineHeaders.input->GetMouseX() > engineHeaders.currentWidth/2-90 && engineHeaders.input->GetMouseX() < engineHeaders.currentWidth/2+90
					&& engineHeaders.input->GetMouseY() > (engineHeaders.currentHeight/2-engineHeaders.font->GetScale()*32.0f*3) && engineHeaders.input->GetMouseY() < (engineHeaders.currentHeight/2-engineHeaders.font->GetScale()*32.0f*2))
			{
				menu->mainMenuSelection = 3;
				menu->currentState = 1;
			}
			if(engineHeaders.input->GetMouseX() > engineHeaders.currentWidth/2-85 && engineHeaders.input->GetMouseX() < engineHeaders.currentWidth/2+85
					&& engineHeaders.input->GetMouseY() > (engineHeaders.currentHeight/2-engineHeaders.font->GetScale()*32.0f*4) && engineHeaders.input->GetMouseY() < (engineHeaders.currentHeight/2-engineHeaders.font->GetScale()*32.0f*3))
			{
				menu->mainMenuSelection = 4;
				return 0;
			}
		}
		break;
	case 1:
		if(engineHeaders.input->GetKeyPressNumber(VK_ESCAPE) > 0)
		{
				menu->currentState = 0;
		}
		break;
	case 2:
		if(engineHeaders.input->GetKeyPressNumber(VK_ESCAPE) > 0)
		{
				menu->currentState = 0;
		}
		if(engineHeaders.input->GetKeyPressNumber(VK_RETURN) > 0)
		{
			engineHeaders.OnLost();
			engineHeaders.window->SetFullscreenState(menu->fullscreen);
			engineHeaders.window->SetSizePermission(true);
			if(engineHeaders.window->GetFullscreenState())
			{
				engineHeaders.device->d3dPPFullScreen.BackBufferWidth = engineHeaders.device->displayModes[0][menu->currentMode].Width;
				engineHeaders.device->d3dPPFullScreen.BackBufferHeight = engineHeaders.device->displayModes[0][menu->currentMode].Height;
				engineHeaders.window->SetWindowWidthAndHeight(engineHeaders.device->displayModes[0][menu->currentMode].Width, engineHeaders.device->displayModes[0][menu->currentMode].Height);
				engineHeaders.window->SetWindowPosition(0,0);
				engineHeaders.device->GetDevice()->Reset(&engineHeaders.device->d3dPPFullScreen);
			}else{
				engineHeaders.device->d3dPPWindowed.BackBufferWidth = engineHeaders.device->displayModes[0][menu->currentMode].Width;
				engineHeaders.device->d3dPPWindowed.BackBufferHeight = engineHeaders.device->displayModes[0][menu->currentMode].Height;
				engineHeaders.window->SetWindowWidthAndHeight(engineHeaders.device->displayModes[0][menu->currentMode].Width, engineHeaders.device->displayModes[0][menu->currentMode].Height);
				engineHeaders.window->SetWindowPosition((int)(GetSystemMetrics(SM_CXSCREEN)/2 - engineHeaders.device->d3dPPWindowed.BackBufferWidth/2), 
				(int)(GetSystemMetrics(SM_CYSCREEN)/2 - engineHeaders.device->d3dPPWindowed.BackBufferHeight/2));
				engineHeaders.device->GetDevice()->Reset(&engineHeaders.device->d3dPPWindowed);
			}
			engineHeaders.window->SetSizePermission(false);
			engineHeaders.currentWidth = engineHeaders.window->GetFullscreenState() ? (float)engineHeaders.device->d3dPPFullScreen.BackBufferWidth : (float)engineHeaders.device->d3dPPWindowed.BackBufferWidth;
			engineHeaders.currentHeight = engineHeaders.window->GetFullscreenState() ? (float)engineHeaders.device->d3dPPFullScreen.BackBufferHeight : (float)engineHeaders.device->d3dPPWindowed.BackBufferHeight;
			engineHeaders.OnReset();
			engineHeaders.scene2d->SetProjMat(engineHeaders.currentWidth, engineHeaders.currentHeight, 1.0f, 100.0f);
			engineHeaders.camera->SetProjectionMatrix(D3DX_PI / 4, (float)(engineHeaders.currentWidth / engineHeaders.currentHeight), 0.1f, 1000.0f);
			menu->AdjustForWidthAndHeight();
			engineHeaders.AdjustHAndW();
		}
		if(engineHeaders.input->GetKeyPressNumber(VK_LBUTTON))
		{
			if(engineHeaders.input->GetMouseX() > engineHeaders.currentWidth/2-200 && engineHeaders.input->GetMouseX() < engineHeaders.currentWidth/2+200
				&& engineHeaders.input->GetMouseY() > engineHeaders.currentHeight/2-(engineHeaders.font->GetScale()*32.0f) && engineHeaders.input->GetMouseY() < engineHeaders.currentHeight/2)
			{
				menu->fullscreen = !menu->fullscreen;
			}
			if(engineHeaders.input->GetMouseX() > engineHeaders.currentWidth/2-200 && engineHeaders.input->GetMouseX() < engineHeaders.currentWidth/2+200
				&& engineHeaders.input->GetMouseY() > engineHeaders.currentHeight/2-(engineHeaders.font->GetScale()*32.0f*2) && engineHeaders.input->GetMouseY() < engineHeaders.currentHeight/2-(engineHeaders.font->GetScale()*32.0f))
			{
				menu->currentMode++;
				if(menu->currentMode == engineHeaders.device->modesForEachFormat[0])
				{
					menu->currentMode = 0;
				}
			}
		}

		break;
	}
	return 1;
}

bool Menu::Render(Menu *menu)
{
	switch(menu->currentState)
	{
	case 0:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawSprite(menu->mainMenuLogo, engineHeaders.currentWidth/2 - menu->mainMenuLogo->GetWidth()/2, engineHeaders.currentHeight*menu->logoy - menu->mainMenuLogo->GetHeight());
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 + engineHeaders.font->GetScale()*32.0f, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Начать игру");
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Построить лабиринт");
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 - engineHeaders.font->GetScale()*32.0f, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Опции");
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 - engineHeaders.font->GetScale()*32.0f*2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Помощь");
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 - engineHeaders.font->GetScale()*32.0f*3, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Выход");
		engineHeaders.scene2d->sceneEnd();
		break;
	case 1:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawSprite(menu->mainMenuLogo, engineHeaders.currentWidth/2 - menu->mainMenuLogo->GetWidth()/2, engineHeaders.currentHeight*menu->logoy - menu->mainMenuLogo->GetHeight());
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2+120, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, 
			L"В игре:\nWASD для перемещения\nОсматриваться с помощью мышки\nОткрыть карту M\nПосчитать маршрут N\nВ редакторе:\nЛевая клавища мыши - поставить стену\n1-начало, 2 - конец\ns-сохранить\nesc - выйти");
		engineHeaders.scene2d->sceneEnd();
		break;
	case 2:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawSprite(menu->mainMenuLogo, engineHeaders.currentWidth/2 - menu->mainMenuLogo->GetWidth()/2, engineHeaders.currentHeight*menu->logoy - menu->mainMenuLogo->GetHeight());
		if(menu->fullscreen)
			engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"На весь экран           Да");
		else
			engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"На весь экран           Нет");
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 - (engineHeaders.font->GetScale()*32.0f), C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Разрешение %ux%u", engineHeaders.device->displayModes[0][menu->currentMode].Width, engineHeaders.device->displayModes[0][menu->currentMode].Height);
		engineHeaders.scene2d->sceneEnd();
		break;
	}
	return true;
}

void Menu::AdjustForWidthAndHeight()
{
	switch((int)engineHeaders.currentWidth)
	{
	case 800:
		this->mainMenuLogo->SetWidth(724.0f);
		this->mainMenuLogo->SetHeight(61.0f);
		this->logoy = 0.95f;
		break;
	case 1024:
		this->mainMenuLogo->SetWidth(764.0f);
		this->mainMenuLogo->SetHeight(65.0f);
		this->logoy = 0.90f;
		break;
	case 1280:
		this->mainMenuLogo->SetWidth(800.0f);
		this->mainMenuLogo->SetHeight(71.0f);
		this->logoy = 0.90f;
		break;
	case 1366:
		this->mainMenuLogo->SetWidth(850.0f);
		this->mainMenuLogo->SetHeight(75.0f);
		this->logoy = 0.88f;
		break;
	case 1600:
		this->mainMenuLogo->SetWidth(921.0f);
		this->mainMenuLogo->SetHeight(81.0f);
		this->logoy = 0.85f;
		break;
	case 1920:
		this->mainMenuLogo->SetWidth(921.0f);
		this->mainMenuLogo->SetHeight(81.0f);
		this->logoy = 0.85f;
		break;
	}
}