#include "IWindows.h"

IWindows::IWindows()
{
	//присваеваем всем переменным значение по умолчанию
	this->RenderFunction = NULL;
	this->LogicFunction = NULL;

	this->hWnd = NULL;

	ZeroMemory(&this->winClass,sizeof(WNDCLASSEX));
	this->winClass.cbClsExtra=NULL;
	this->winClass.cbSize=sizeof(WNDCLASSEX);
	this->winClass.cbWndExtra=NULL;
	this->winClass.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	this->winClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	this->winClass.hIcon=NULL;
	this->winClass.hIconSm=NULL;
	this->winClass.hInstance= NULL;
	this->winClass.lpfnWndProc = (WNDPROC)MsgProc;
	this->winClass.lpszClassName = L"CeruleanEngineWindowClass";
	this->winClass.lpszMenuName=NULL;
	this->winClass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_OWNDC;

	this->windowWidth = 800;
	this->windowHeight = 600;

	this->lpszWinName = L"CeruleanEngine";
	this->winRect = new RECT;
	this->winRect->left = (int)(GetSystemMetrics(SM_CXSCREEN)/2 - this->windowWidth/2);
	this->winRect->top = (int)(GetSystemMetrics(SM_CYSCREEN)/2 - this->windowHeight/2);
	this->winRect->right = this->winRect->left+windowWidth;
	this->winRect->bottom = this->winRect->top+windowHeight;
	
	this->winStyle = WS_CAPTION | WS_SYSMENU;
	this->fullscreenWindowStyle = WS_POPUP;
	AdjustWindowRect(this->winRect,this->winStyle,false);

	this->allowSizeChanging = false;
	this->allowMoving = true;
	this->focused= true;
	this->fullscreen = false;

	this->FPS = 60;
	this->timePerFrame = (DWORD)(1000 / this->FPS);
}

IWindows::IWindows(int windowWidth, int windowHeight, bool fullscreen)
{
	//присваеваем всем переменным значение по умолчанию
	this->RenderFunction = NULL;
	this->LogicFunction = NULL;

	this->hWnd = NULL;

	ZeroMemory(&this->winClass,sizeof(WNDCLASSEX));
	this->winClass.cbClsExtra=NULL;
	this->winClass.cbSize=sizeof(WNDCLASSEX);
	this->winClass.cbWndExtra=NULL;
	this->winClass.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	this->winClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	this->winClass.hIcon=NULL;
	this->winClass.hIconSm=NULL;
	this->winClass.hInstance= NULL;
	this->winClass.lpfnWndProc = (WNDPROC)MsgProc;
	this->winClass.lpszClassName = L"CeruleanEngineWindowClass";
	this->winClass.lpszMenuName=NULL;
	this->winClass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_OWNDC;

	this->winRect = new RECT;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	GetWindowRect(GetDesktopWindow(),this->winRect);
	this->fullscreenWindowWidth = this->winRect->right;
	this->fullscreenWindowHeight = this->winRect->bottom;
	this->fullscreenWindowStyle = WS_POPUP;
	this->winStyle =  WS_CAPTION | WS_SYSMENU;
	this->lpszWinName = L"The labyrinth";

	if(!fullscreen)
	{
		this->winRect->left = (int)(GetSystemMetrics(SM_CXSCREEN)/2 - this->windowWidth/2);
		this->winRect->top = (int)(GetSystemMetrics(SM_CYSCREEN)/2 - this->windowHeight/2);
		this->winRect->right = this->winRect->left+windowWidth;
		this->winRect->bottom = this->winRect->top+windowHeight;
		AdjustWindowRect(this->winRect,this->winStyle,false);
	}
	this->allowSizeChanging = false;
	this->allowMoving = true;
	this->focused= true;
	this->fullscreen = fullscreen;
	this->FPS = 60;
	this->timePerFrame = (DWORD)(1000 / this->FPS);
}

bool IWindows::CreateWnd(HINSTANCE hInst)
{
	this->winClass.hInstance = hInst;
	//регистрируем класс для создания окна
	if(RegisterClassEx( &this->winClass) == NULL)
	{
		eLOG(L"FAILED to register window class (error code:%u)",GetLastError());
		return false;
	}
	//создаем окно
	if((hWnd = CreateWindow(this->winClass.lpszClassName,this->lpszWinName,this->winStyle,this->winRect->left,this->winRect->top,this->winRect->right-this->winRect->left,this->winRect->bottom-this->winRect->top,NULL,NULL,this->winClass.hInstance,NULL)) == NULL)
	{
		eLOG(L"FAILED to create a window(error code:%u)",GetLastError());
		return false;
	}else
		LOG(L"A window was succesfully created");
	ShowWindow(this->hWnd, SW_SHOWDEFAULT); //показываем окно
	//обновляем окно
	UpdateWindow(this->hWnd);
	return true;
}

bool IWindows::DestroyWnd()
{
	if(this->hWnd != NULL)
		if(DestroyWindow(this->hWnd) == NULL)
		{
			eLOG(L"FAILED to destroy the window(error code:%d)",GetLastError());
			return false;
		}
	//отменяем регистрацию класса
	if(UnregisterClass(this->winClass.lpszClassName,this->winClass.hInstance) == NULL)
	{
		eLOG(L"FAILED to unregister window class(error code:%d)",GetLastError());
		return false;
	}
	return true;	
}

bool IWindows::IWindowsMessageDispatcher(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_MOVE:
		{
			if(this->fullscreen)
				return true;
			if(this->allowMoving)
			{
				this->winRect->left = LOWORD(lParam);
				this->winRect->top = HIWORD(lParam);
				this->winRect->right = this->winRect->left + this->windowWidth;
				this->winRect->bottom = this->winRect->top + this->windowHeight;
			}else
			{
				SetWindowPos(this->hWnd,HWND_TOP,this->winRect->left,this->winRect->top,this->winRect->right-this->winRect->left,this->winRect->bottom-this->winRect->top,NULL);
			}
			return true;
		}
	case WM_SIZE:
		{
			if(this->fullscreen)
				return true;
			if(this->allowSizeChanging)
			{
				this->winRect->right = LOWORD(lParam) + this->winRect->left;
				this->winRect->bottom = HIWORD(lParam) + this->winRect->top;
			}else
			{
				SetWindowPos(this->hWnd,HWND_TOP,this->winRect->left,this->winRect->top,this->winRect->right-this->winRect->left,this->winRect->bottom-this->winRect->top,NULL);
			}
			return true;
		}
	case WM_ACTIVATE:
		{
			if(LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
				this->focused = true;
			else
				this->focused = false;
			return true;
		}
	}
	return false;
}

void IWindows::Run()
{
	LOG(L"Entering IWindows::Run()");
	bool bBreak = false;
	//структура для работы с очередью сообщений 
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );
	
	//начальные установки для работы со временем
	timeBeginPeriod(1);
	this->cumulativeTime = timeGetTime();
	this->FPSCounterTimer = 0;
	this->frames = 0;
	while(1)
	{
		this->startTime = timeGetTime();

		//message queue
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)!=NULL)
		{
			if(msg.message == WM_QUIT)
			{
				bBreak = true;
				break;
			}
			DispatchMessage(&msg);
		}
		if(bBreak)
			break;

		//Если не прошло достаточно времени, то ждем
		if((this->startTime-this->cumulativeTime) < this->timePerFrame)
			Sleep(this->timePerFrame - (this->startTime - this->cumulativeTime));
		
		this->FPSCounterTimer += this->startTime - this->cumulativeTime + this->timePerFrame;
		this->cumulativeTime = timeGetTime();
		//считаем фпс
		this->frames += 1;
		if(this->FPSCounterTimer >= 1000)
		{
			this->realFPS = (float)(this->frames * 1000 / this->FPSCounterTimer);
			this->FPSCounterTimer = 0; 
			this->frames = 0;
		}

		//выполняем функции логики и рендера, если одна из функции возвратила false, выходим из цикла
        if(LogicFunction() == false)
			break;
		if(RenderFunction() == false)
			break;
	}
	timeEndPeriod(1);
}

IWindows::~IWindows()
{
	delete this->winRect;
	this->DestroyWnd();
}

void IWindows::SetWindowWidthAndHeight(int width, int height)
{
	if(!this->allowSizeChanging)
		return;
	this->winRect->right = this->winRect->left + width;
	this->winRect->bottom = this->winRect->top + height;
	AdjustWindowRect(this->winRect,this->winStyle,false);
	this->windowWidth = (int)(this->winRect->right - this->winRect->left);
	this->windowHeight = (int)(this->winRect->bottom - this->winRect->top);
	SetWindowPos(this->hWnd, HWND_TOP, 0, 0, this->winRect->right - this->winRect->left, this->winRect->bottom - this->winRect->top, SWP_NOMOVE);
}

void IWindows::SetWindowPosition(int x, int y)
{
	if(!this->allowMoving)
		return;
	this->winRect->left = x;
	this->winRect->top = y;
	this->winRect->right = this->winRect->left + this->windowWidth;
	this->winRect->bottom = this->winRect->top + this->windowHeight;
	SetWindowPos(this->hWnd, HWND_TOP, this->winRect->left, this->winRect->top, 0, 0, SWP_NOSIZE);
}

void IWindows::SetFullscreenState(bool fullscreen)
{
	if(this->fullscreen == fullscreen)
		return;
	if(fullscreen)
	{
		SetWindowLongPtr(this->hWnd,GWL_STYLE,this->fullscreenWindowStyle);
		this->fullscreen = true;
		SetWindowPos(this->hWnd,HWND_TOP,0,0,this->fullscreenWindowWidth,this->fullscreenWindowHeight,SWP_HIDEWINDOW);
		this->winRect->left = 0;
		this->winRect->top = 0;
		this->winRect->right = this->fullscreenWindowWidth;
		this->winRect->bottom = this->fullscreenWindowHeight;
	}else{
		SetWindowLongPtr(this->hWnd,GWL_STYLE,this->winStyle);
		this->fullscreen = false;
		this->winRect->left = 0;
		this->winRect->top = 0;
		this->winRect->right = (LONG)(GetSystemMetrics(SM_CXSCREEN)/2 - this->windowWidth/2);
		this->winRect->bottom = (LONG)(GetSystemMetrics(SM_CYSCREEN)/2 - this->windowHeight/2);
		SetWindowPos(this->hWnd,HWND_TOP,this->winRect->left,this->winRect->top,this->winRect->right-this->winRect->left,this->winRect->bottom-this->winRect->top,SWP_HIDEWINDOW);
	}
	ShowWindow(this->hWnd, SW_SHOWDEFAULT);
}

