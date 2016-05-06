#ifndef IWINDOWS_H
#define IWINDOWS_H

#include "ILogger.h"

//class releases connections between application and os Windows
class IWindows
{
	HWND hWnd; //���������� ����
	WNDCLASSEX winClass; //���������, ���������� ���������� �� ������ ����
	LPCTSTR	lpszWinName; //�������� ����
	RECT *winRect;
	DWORD winStyle; // ����� ����
	DWORD fullscreenWindowStyle;
	bool focused; //���������� �� ���� � ������ �����?
	bool allowSizeChanging; //����� �� ������ ������ ����
	bool allowMoving; // ����� �� ������� ����
	int	windowWidth;
	int	windowHeight;
	int	fullscreenWindowWidth;
	int	fullscreenWindowHeight;
	bool fullscreen;

	//������ � FPS
	DWORD startTime;
	DWORD deltaTime;
	DWORD cumulativeTime;
	DWORD FPSCounterTimer;
	DWORD timePerFrame;
	unsigned int FPS;
	unsigned int frames;
	float realFPS;
public:
	void SetFPS(int _nFPS);
	bool IWindowsMessageDispatcher(UINT msg, WPARAM wParam, LPARAM lParam);
	bool GetFocusState() { return this->focused; }
	void SetMovePermission(bool allowMoving) { this->allowMoving = allowMoving; }
	bool GetMovePermission() { return this->allowMoving; }
	void SetSizePermission(bool allowSize) { this->allowSizeChanging = allowSize; }
	bool GetSizePermission() { return this->allowSizeChanging; }
	int	GetWindowWidth() { return (this->fullscreen ? this->fullscreenWindowWidth:this->windowWidth); }
	int	GetWindowHeight() { return (this->fullscreen ? this->fullscreenWindowHeight:this->windowHeight); }
	void GetWindowPos(int &x, int &y) { x = this->winRect->left; y = this->winRect->top; }
	int GetWindowX() { return this->winRect->left; }
	int GetWindowY() { return this->winRect->top; }
	void SetWindowWidthAndHeight(int width, int height);
	void SetWindowPosition(int x, int y);
	bool GetFullscreenState() { return this->fullscreen; }
	void SetFullscreenState(bool fullscreen);
	HWND GetWindowHandler() { return this->hWnd; }
	void SetHWND(HWND hWnd){ this->hWnd = hWnd; }

	//��������� �� ������� ������ � �������
	bool (*RenderFunction)();
	bool (*LogicFunction)();

	//������� ��� �������� � �������� ����
	bool CreateWnd(HINSTANCE hInst);
	bool DestroyWnd();

	//������� ����
	void Run();

	//����������� � ��c�������
	IWindows();
	IWindows(int windowWidth, int windowHeight, bool fullscreen = false);
	~IWindows();
};

#endif