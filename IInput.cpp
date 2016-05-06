#include "IInput.h"

IInput::IInput(HWND	hWnd)
{
	ZeroMemory(&this->keyState,sizeof(int)*256);
	GetCursorPos(&this->mouseCoord);
	ScreenToClient(hWnd,&this->mouseCoord);
	this->keyBeg = NULL;
	this->keyEnd = NULL;
	this->mouseBeg = NULL;
	this->mouseEnd = NULL;
	this->hWnd = hWnd;
	this->mouseClipped = false;
	oldMouseCoord.x = oldMouseCoord.y = 0;
}

IInput::~IInput()
{
	ClearInput();
}

void IInput::ClearInput()
{
	if(keyBeg != NULL)
	{
		while(keyBeg != NULL)
		{
			keyEnd = keyBeg->next;
			delete keyBeg;
			keyBeg = keyEnd;
		}
	}

	if(mouseBeg != NULL)
	{
		while(mouseBeg != NULL)
		{
			mouseEnd = mouseBeg->next;
			delete mouseBeg;
			mouseBeg = mouseEnd;
		}
	}

	ZeroMemory(&this->keyState,sizeof(int)*256);
}

void IInput::AddToMouseQueue(WPARAM wParam, LPARAM lParam, int virtualcode)
{
	GetClientRect(this->hWnd,&this->rect);
	if(this->mouseBeg == NULL)
	{
		this->mouseBeg = new MousePressQueue;
		this->mouseBeg->next = NULL;
		this->mouseBeg->virtualCode = (virtualcode == -1) ? wParam : virtualcode;
		this->mouseBeg->mouseCoord.x = LOWORD(lParam);
		this->mouseBeg->mouseCoord.y = (this->rect.bottom - this->rect.top) - HIWORD(lParam);
	}else
	{
		this->mouseEnd->next = new MousePressQueue;
		this->mouseEnd->next->virtualCode = (virtualcode == -1) ? wParam : virtualcode;
		this->mouseEnd->next->mouseCoord.x = LOWORD(lParam);
		this->mouseEnd->next->mouseCoord.y = (this->rect.bottom - this->rect.top) - HIWORD(lParam);
		this->mouseEnd = mouseEnd->next;
	}
}

bool IInput::IInputMessageDispatcher(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_MOUSEMOVE:
		{
			GetClientRect(this->hWnd,&this->rect);
			this->oldMouseCoord.x = this->mouseCoord.x;
			this->oldMouseCoord.y = this->mouseCoord.y;
			this->mouseCoord.x = LOWORD(lParam);
			this->mouseCoord.y = (rect.bottom - rect.top) - HIWORD(lParam);
			return true;
		}
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		{
			this->keyState[wParam] += LOWORD(lParam); 
			for(int i = 0; i < LOWORD(lParam); i++)
			{
				if(this->keyBeg == NULL)
				{
					this->keyBeg = new KeyPressQueue;
					this->keyBeg->next = NULL;
					this->keyBeg->virtualCode = wParam;
					this->keyBeg->scanCode = LOBYTE(HIWORD(lParam));
					this->keyEnd = this->keyBeg;
				}else
				{
					this->keyEnd->next = new KeyPressQueue;
					this->keyEnd->next->virtualCode = wParam;
					this->keyEnd->next->scanCode = LOBYTE(HIWORD(lParam));
					this->keyEnd = keyEnd->next;
					this->keyEnd->next = NULL;
				}
			}
			return true;
		}
	case WM_LBUTTONDOWN:
		{
			this->keyState[VK_LBUTTON] += 1; 
			this->AddToMouseQueue(wParam,lParam);
			return true;
		}
	case WM_RBUTTONDOWN:
		{
			this->keyState[VK_RBUTTON] += 1; 
			this->AddToMouseQueue(wParam,lParam);
			return true;
		}
	case WM_MBUTTONDOWN:
		{
			this->keyState[VK_MBUTTON] += 1; 
			this->AddToMouseQueue(wParam,lParam);
			return true;
		}
	case WM_MOUSEWHEEL:
		{
			this->AddToMouseQueue(wParam,lParam,7);
			return true;
		}
	case WM_XBUTTONDOWN:
		{
			(HIWORD(wParam) == XBUTTON1) ? this->keyState[VK_XBUTTON1] += 1 : this->keyState[VK_XBUTTON2] += 1; 
			this->AddToMouseQueue(wParam,lParam,(HIWORD(wParam) == XBUTTON1) ? 5 : 6);
			return true;
		}
	case WM_LBUTTONDBLCLK:
		{
			this->AddToMouseQueue(wParam,lParam,8);
			return true;
		}
	case WM_RBUTTONDBLCLK:
		{
			this->AddToMouseQueue(wParam,lParam,9);
			return true;
		}
	case WM_MBUTTONDBLCLK:
		{
			this->AddToMouseQueue(wParam,lParam,10);
			return true;
		}
	}
	return false;
}

bool IInput::GetCurrentKeyState(unsigned int VirtualKeyCode) {
	if((GetKeyState(VirtualKeyCode) & 0x80)>0)
		return true;
	return false;
}

unsigned int IInput::GetKeyPressNumber(unsigned int VirtualKeyCode)
{
	return keyState[VirtualKeyCode];
}

wchar_t IInput::GetUnicodeFromKeyQueue()
{
	if(this->keyBeg != NULL)
	{
		wchar_t buffer;
		KeyPressQueue *temp;
		BYTE KeySt[256];
		GetKeyboardState(KeySt);
		ToUnicode(this->keyBeg->virtualCode,this->keyBeg->scanCode,KeySt,&buffer,1,NULL);
		temp = this->keyBeg->next;
		if(this->keyBeg == this->keyEnd)
			this->keyEnd = NULL;
		delete this->keyBeg;
		this->keyBeg = temp;
		return buffer;
	}else
		return NULL;
}

unsigned int IInput::GetVirtualCodeOfKeyboardInput()
{
	if(this->keyBeg != NULL)
	{
		unsigned int VK;
		KeyPressQueue *temp;
		VK = this->keyBeg->virtualCode;
		temp = this->keyBeg->next;
		if(this->keyBeg == this->keyEnd)
			this->keyEnd = NULL;
		delete this->keyBeg;
		this->keyBeg = temp;
		return VK;
	}else
		return 0;
}

unsigned int IInput::GetVirtualCodeOfMouseInput()
{
	if(this->mouseBeg != NULL)
	{
		unsigned int VK;
		MousePressQueue *temp;
		VK = this->mouseBeg->virtualCode;
		temp = this->mouseBeg->next;
		if(this->mouseBeg == this->mouseEnd)
			this->mouseEnd = NULL;
		delete this->mouseBeg;
		this->mouseBeg = temp;
		return VK;
	}else
		return 0;
}

unsigned int IInput::GetVirtualCodeOfMouseInput(int &MouseX, int &MouseY)
{
	if(this->mouseBeg != NULL)
	{
		unsigned int VK;
		MousePressQueue *temp;
		VK = this->mouseBeg->virtualCode;
		MouseX = this->mouseBeg->mouseCoord.x;
		MouseY = this->mouseBeg->mouseCoord.y;
		temp = this->mouseBeg->next;
		if(this->mouseBeg == this->mouseEnd)
			this->mouseEnd = NULL;
		delete this->mouseBeg;
		this->mouseBeg = temp;
		return VK;
	}else
		return 0;
}

void IInput::ClipMouse()
{
	GetClipCursor(&this->oldRect);
	GetWindowRect(this->hWnd, &this->rect);
	ClipCursor(&this->rect);
	this->mouseClipped = true;
}

void IInput::UnclipMouse()
{
	if(this->mouseClipped)
		ClipCursor(&this->oldRect);
}

int IInput::GetXRelative()
{
	return this->mouseCoord.x - this->oldMouseCoord.x;
}

int IInput::GetYRelative()
{
	return this->mouseCoord.y - this->oldMouseCoord.y;
}
