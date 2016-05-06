#ifndef DEFINITIONSANDINCLUDE_H
#define DEFINITIONSANDINCLUDE_H

//memory leak check
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

//windows headers
#include <windows.h>
LRESULT CALLBACK MsgProc(HWND HWND, UINT uMsg, WPARAM wParam, LPARAM lParam); 

//directx headers
#define D3D_DEBUG_INFO
#include <d3d9.h>
#include <d3dx9.h>

// c/c++ runtime library
#include <cstdio>
#include <cstdlib>
#include <wchar.h>
#include <ctime>

//file input/output
#include <fstream>

//containers
#include <list>
#include <vector>

//resources
#include "resource.h"

#include "squall.h"

#define SAFE_RELEASE(pointer) { if (pointer != NULL) { delete (pointer);    (pointer)=NULL; } }
#define SAFE_ARRAY_RELEASE(pointer) { if (pointer != NULL) { delete [](pointer);    (pointer)=NULL; } }
#define SAFE_COM_RELEASE(pointer) { if (pointer != NULL) { pointer->Release();    (pointer)=NULL; } }

template <class T>
class SmartPointer
{
	T* pointer;
	DWORD *counter;
	bool isArray;
public:
	SmartPointer(T* data, bool isArray = false) { this->pointer = data; this->counter = new DWORD; *this->counter = 0; this->isArray = isArray;}
	T* GetData() { return this->pointer; }
	SmartPointer(SmartPointer &right) { 
		(*right.counter)++;
		this->counter = right.counter;
		this->pointer = right.pointer; 
		this->isArray = right.isArray;
	}
	~SmartPointer() { 
		if(*this->counter == 0) 
		{
			this->isArray ? delete []this->pointer : delete this->pointer;
			delete this->counter;
		}
		else (*this->counter)--; 
	}
};

template <class T>
class SmartPointerCOM
{
	T* pointer;
	DWORD *counter;
public:
	SmartPointerCOM(T* data, bool isArray = false) { this->pointer = data; this->counter = new DWORD; *this->counter = 0;}
	T* GetData() { return this->pointer; }
	SmartPointerCOM(SmartPointerCOM &right) { 
		(*right.counter)++;
		this->counter = right.counter;
		this->pointer = right.pointer; 
	}
	~SmartPointerCOM() { 
		if(*this->counter == 0) 
		{
			this->pointer->Release();
			delete this->counter;
		}
		else (*this->counter)--; 
	}
};

#endif