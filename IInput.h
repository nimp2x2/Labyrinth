#include "ILogger.h"

#ifndef IINPUT_H
#define IINPUT_H

class KeyPressQueue
{
public:
	unsigned int scanCode;
	unsigned int virtualCode;
	KeyPressQueue *next;
};

class MousePressQueue
{
public:
	POINT mouseCoord;
	unsigned int virtualCode;
	MousePressQueue *next;
};

class IInput
{
public:
	bool GetCurrentKeyState(unsigned int virtualKeyCode); // просто возвращает нажата ли клавиша или нет
	unsigned int GetKeyPressNumber(unsigned int virtualKeyCode); // возвращает количество нажатий на клавишу

	wchar_t GetUnicodeFromKeyQueue(); // возвращает символы клавиш в том порядке, в котором они были нажаты
	unsigned int GetVirtualCodeOfKeyboardInput(); // возвращает виртуальный символ клавиш в том порядке, в котором они они были нажаты
	unsigned int GetVirtualCodeOfMouseInput(); // возвращает код клавиш мыши в том порядке, в котором они были нажаты
	// virtual key codes for mouse input
	// 1 - left mouse button
	// 2 - right mouse button
	// 4 - middle mouse button
	// 7 - wheel, x = distance, y = 0;
	// 8 - left mouse  button dbl click
	// 9 - right mouse button dbl click
	// 10 - middle mouse button dbl click
	// note what WM_LMOUSEDBLCLICK also generates two WM_LBUTTONDOWN
	// 5 - first x button
	// 6 - second x button
	unsigned int GetVirtualCodeOfMouseInput(int &mouseX, int &mouseY); //аналогично предыдущему, но в добавок возвращает координату нажатия клавиши

	bool IInputMessageDispatcher(UINT msg, WPARAM wParam, LPARAM lParam);
	void ClearInput();
	void SetHWND(HWND hWnd) { this->hWnd = hWnd; }
	int	GetMouseX(){ return this->mouseCoord.x; }
	int	GetMouseY(){ return this->mouseCoord.y; }
	void ClipMouse();
	void UnclipMouse();
	int GetXRelative();
	int GetYRelative();

	IInput(HWND hWnd);
	~IInput();
private:
	HWND hWnd;
	unsigned int keyState[256];	
	POINT mouseCoord;
	POINT oldMouseCoord;
	RECT rect;
	RECT oldRect;
	bool mouseClipped;

	KeyPressQueue *keyBeg;
	KeyPressQueue *keyEnd;
	MousePressQueue	*mouseBeg;
	MousePressQueue	*mouseEnd;
	void AddToMouseQueue(WPARAM wParam, LPARAM lParam, int virtualcode = -1);
};
#endif