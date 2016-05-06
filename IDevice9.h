#ifndef IDEVICE9_H
#define IDEVICE9_H

#include "ILogger.h"

class IDevice9
{	
	LPDIRECT3D9						d3d;
	LPDIRECT3DDEVICE9				d3dDevice; 
	D3DVIEWPORT9					d3dViewPort;
	HWND							hWnd;
	HINSTANCE						hInst;
	UINT							adapter;
	D3DFORMAT						*surfaceFormat;
	int								surfaceFormatCount;
	int								*neededWidth;
	int								*neededHeight;
	int								neededScreenDimensionCount;
public:
	int								*modesForEachFormat;
	D3DDISPLAYMODE					**displayModes;
	D3DCAPS9						deviceCaps;
	D3DPRESENT_PARAMETERS			d3dPPFullScreen;
	D3DPRESENT_PARAMETERS			d3dPPWindowed; 
	bool 							CreateDirectX(); 
	bool							CreateDirectXDevice(bool bWindowed = false);
	LPDIRECT3DDEVICE9				GetDevice() { return this->d3dDevice; }
	LPDIRECT3D9						GetD3D() { return this->d3d; }
	bool							ChooseAdapter();
	bool							CheckBackBufferFormat(D3DFORMAT *surfaceFormatToCheck, int count);
	bool							ChooseAndCheckDisplayMode(int *width, int *height, int count);
	bool							ChooseAndCheckMultisample();
	bool							CheckDevCaps();


	IDevice9(HWND hWnd, HINSTANCE hInst);						
	~IDevice9();
};

#endif