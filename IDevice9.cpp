#include "IDevice9.h"

INT_PTR CALLBACK	ChooseDeviceDlg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	
int	selectionIndex;
HWND hComboBox;
D3DADAPTER_IDENTIFIER9* adapterId;
wchar_t *buffer;

IDevice9::IDevice9(HWND hWnd, HINSTANCE hInst)
{
	this->hWnd = hWnd;
	this->d3d = NULL;
	this->d3dDevice = NULL;
	this->hInst = hInst;
	this->displayModes = NULL;
	this->surfaceFormatCount = 0;
	this->surfaceFormat = NULL;
	this->modesForEachFormat = NULL;
	this->neededScreenDimensionCount = 0;
	this->neededHeight = NULL;
	this->neededWidth = NULL;

	// заполн€ем структуру D3DPRESENTPARAMETERS9 дл€ полноэкранного режима
	ZeroMemory( &this->d3dPPFullScreen, sizeof( this->d3dPPFullScreen ) );
	this->d3dPPFullScreen.Windowed = FALSE;
	this->d3dPPFullScreen.SwapEffect = D3DSWAPEFFECT_DISCARD;
	this->d3dPPFullScreen.BackBufferFormat = D3DFMT_X8R8G8B8;
	this->d3dPPFullScreen.BackBufferWidth = 800;
	this->d3dPPFullScreen.BackBufferHeight = 600;
	this->d3dPPFullScreen.BackBufferCount = 1;
	this->d3dPPFullScreen.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	this->d3dPPFullScreen.FullScreen_RefreshRateInHz = 60;
	this->d3dPPFullScreen.EnableAutoDepthStencil = true;
	this->d3dPPFullScreen.AutoDepthStencilFormat = D3DFMT_D16;
	this->d3dPPFullScreen.Flags = 0;
	this->d3dPPFullScreen.MultiSampleType = D3DMULTISAMPLE_NONE;
	this->d3dPPFullScreen.MultiSampleQuality = 0;
	//this value must be set if using fullscreen render
	this->d3dPPFullScreen.hDeviceWindow = NULL;

	// заполн€ем структуру D3DPRESENTPARAMETERS9 дл€ оконного режима
	ZeroMemory( &this->d3dPPWindowed, sizeof( this->d3dPPWindowed ) );
	this->d3dPPWindowed.Windowed = TRUE;
	this->d3dPPWindowed.SwapEffect = D3DSWAPEFFECT_DISCARD;
	this->d3dPPWindowed.BackBufferFormat = D3DFMT_UNKNOWN;
	this->d3dPPWindowed.BackBufferWidth = 800;
	this->d3dPPWindowed.BackBufferHeight = 600;
	this->d3dPPWindowed.BackBufferCount = 1;
	this->d3dPPWindowed.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	this->d3dPPWindowed.FullScreen_RefreshRateInHz = 0;
	this->d3dPPWindowed.EnableAutoDepthStencil = true;
	this->d3dPPWindowed.AutoDepthStencilFormat = D3DFMT_D16;
	this->d3dPPWindowed.Flags = 0;
	this->d3dPPWindowed.MultiSampleType = D3DMULTISAMPLE_NONE;
	this->d3dPPWindowed.MultiSampleQuality = 0;
	this->d3dPPWindowed.hDeviceWindow = NULL;
}

IDevice9::~IDevice9()
{
	SAFE_ARRAY_RELEASE(this->neededHeight);
	SAFE_ARRAY_RELEASE(this->neededWidth);
	SAFE_ARRAY_RELEASE(this->modesForEachFormat);
	SAFE_ARRAY_RELEASE(this->surfaceFormat);
	if(this->displayModes != NULL)
	{
		for(int i = 0; i < this->surfaceFormatCount; i++)
			SAFE_ARRAY_RELEASE(this->displayModes[i]);
		delete []this->displayModes;
	}
	SAFE_ARRAY_RELEASE(this->modesForEachFormat);
	SAFE_COM_RELEASE(this->d3dDevice);
	SAFE_COM_RELEASE(this->d3d);
}

bool IDevice9::CreateDirectX()
{
	if(FAILED(this->d3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		eLOG(L"FAILED to create directx");
		return false; 
	}
	else
	{
		LOG(L"DirectX was successfully created");
		return true;
	}
}

bool IDevice9::ChooseAdapter()
{
	if(this->d3d == NULL)
		return false;
	int adapterCount = (int)this->d3d->GetAdapterCount();
	adapterId = new D3DADAPTER_IDENTIFIER9[adapterCount];
	for(int i = 0; i < adapterCount; i++)
		if(this->d3d->GetAdapterIdentifier(i,NULL,&adapterId[i]) != D3D_OK)
		{
			eLOG(L"FAILED to get adapter identifier(%d)",i);
			return false;
		}

	if(adapterCount > 1)
	{
		selectionIndex = adapterCount;
		DialogBox(this->hInst, MAKEINTRESOURCE(IDD_DIALOGBAR), this->hWnd, ChooseDeviceDlg);
		this->adapter = selectionIndex;
		eLOG(L"Using adapter number %d", selectionIndex);
	}else{
		this->adapter = D3DADAPTER_DEFAULT;
		LOG(L"Using default adapter");
		delete []adapterId;
	}
	return true;
}

INT_PTR CALLBACK ChooseDeviceDlg(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (msg)
	{
	case WM_INITDIALOG:
		hComboBox = GetDlgItem(hDlg,IDC_VASCOMBO);
		buffer = new wchar_t[512];
		buffer[511]= 0;
		for(int i = 0; i < selectionIndex; i++)
		{
			MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,adapterId[i].Description,-1,buffer,512);
			SendMessage(hComboBox,CB_ADDSTRING,0,(LPARAM)buffer);
			if(i == 0)
				SendMessage(hComboBox,CB_SETCURSEL,0,(LPARAM)buffer);
		}
		delete []adapterId;
		delete []buffer;
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if(LOWORD(wParam) == IDC_VASBUTTON && HIWORD(wParam) == BN_CLICKED)
		{
			selectionIndex = SendMessageW(hComboBox,CB_GETCURSEL,0,0);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}

bool IDevice9::CheckBackBufferFormat(D3DFORMAT *surfaceFormatToCheck, int count)
{
	if(this->d3d == NULL)
		return false;
	for(int i = 0; i < count; i++)
	{
		if(this->d3d->CheckDeviceType(this->adapter,D3DDEVTYPE_HAL,surfaceFormatToCheck[i],surfaceFormatToCheck[i],false) == D3D_OK 
			&& this->d3d->CheckDeviceType(this->adapter,D3DDEVTYPE_HAL,surfaceFormatToCheck[i],surfaceFormatToCheck[i],true)== D3D_OK)
			this->surfaceFormatCount++;
	}
	if(this->surfaceFormatCount == 0)
	{
		eLOG(L"Supported backbuffer format was not found");
		return false;
	}
	this->surfaceFormat = new D3DFORMAT[this->surfaceFormatCount];
	for(int i = 0; i < count; i++)
	{
		if(this->d3d->CheckDeviceType(this->adapter,D3DDEVTYPE_HAL,surfaceFormatToCheck[i],surfaceFormatToCheck[i],false) == D3D_OK 
			&& this->d3d->CheckDeviceType(this->adapter,D3DDEVTYPE_HAL,surfaceFormatToCheck[i],surfaceFormatToCheck[i],true)== D3D_OK)
			this->surfaceFormat[i]  = surfaceFormatToCheck[i]; 
	}
	this->d3dPPFullScreen.BackBufferFormat = this->surfaceFormat[this->surfaceFormatCount-1];
	return true;
}

bool IDevice9::ChooseAndCheckDisplayMode(int *width, int *height, int count)
{
	this->neededScreenDimensionCount = count;
	this->neededHeight = new int[count];
	this->neededWidth = new int[count];
	for(int i = 0; i < count; i++)
	{
		this->neededHeight[i] = height[i];
		this->neededWidth[i] = width[i];
	}

	if(this->d3d == NULL)
		return false;
	
	int adapterModeCount = 0;
	this->modesForEachFormat = new int[this->surfaceFormatCount];
	bool checker = false;
	for(int i = 0; i < this->surfaceFormatCount; i++)
	{
		adapterModeCount = this->d3d->GetAdapterModeCount(this->adapter,this->surfaceFormat[i]);
		this->modesForEachFormat[i] = adapterModeCount;
	}	
	for(int i = 0; i < this->surfaceFormatCount; i++)
		if(this->modesForEachFormat[i] != 0)
			checker = true;
	if(!checker)
	{
		eLOG(L"Supported display mode was not found");
		return false;
	}
	this->displayModes = new D3DDISPLAYMODE*[this->surfaceFormatCount];
	bool neededWidthAndHeightFound = false;
	int prevCount = 0;
	D3DDISPLAYMODE *tempDisplayMode;
	int j = 0;
	int k = 0;
	checker = false;
	for(int i = 0; i < this->surfaceFormatCount; i++)
	{
		this->displayModes[i] = new D3DDISPLAYMODE[this->modesForEachFormat[i]];
		for(j = 0; j < this->modesForEachFormat[i]; j++)
		{
			this->d3d->EnumAdapterModes(this->adapter,this->surfaceFormat[i],j,&displayModes[i][j]);
			neededWidthAndHeightFound = false;
			for(k = 0; k < this->neededScreenDimensionCount; k++)
			{
				if(this->neededWidth[k] == this->displayModes[i][j].Width && this->neededHeight[k] == this->displayModes[i][j].Height)
				{
					neededWidthAndHeightFound = true;
					break;
				}
			}
			if(!neededWidthAndHeightFound)
				this->displayModes[i][j].Width = this->displayModes[i][j].Height = 0;
		}
		prevCount = this->modesForEachFormat[i];
		for(j = 0; j < prevCount; j++)
		{
			if(this->displayModes[i][j].Width == 0)
				modesForEachFormat[i]--;
		}

		if(modesForEachFormat[i] > 0)
		{
			tempDisplayMode = new D3DDISPLAYMODE[modesForEachFormat[i]];
			k = 0;
			for(j = 0; j < prevCount; j++)
			{
				if(this->displayModes[i][j].Width != 0)
				{
					tempDisplayMode[k] = this->displayModes[i][j];
					k++;
				}
			}
			SAFE_ARRAY_RELEASE(this->displayModes[i]);
			this->displayModes[i] = tempDisplayMode;
			checker = true;
			
		}else{
			SAFE_ARRAY_RELEASE(this->displayModes[i]);
		}
	}
	
	if(!checker)
	{
		eLOG(L"Matching display mode supported by adapter was not found");
		return false;
	}
	for(int i = this->surfaceFormatCount-1; i >= 0; i--)
	{
		if(this->modesForEachFormat[i] != 0)
		{
			this->d3dPPFullScreen.BackBufferWidth = this->displayModes[i][modesForEachFormat[i]-1].Width;
			this->d3dPPFullScreen.BackBufferHeight = this->displayModes[i][modesForEachFormat[i]-1].Height;
			this->d3dPPFullScreen.FullScreen_RefreshRateInHz = this->displayModes[i][modesForEachFormat[i]-1].RefreshRate;
			this->d3dPPWindowed.BackBufferWidth = this->displayModes[i][modesForEachFormat[i]-1].Width;
			this->d3dPPWindowed.BackBufferHeight = this->displayModes[i][modesForEachFormat[i]-1].Height;
			break;
		}
	}
	return true;
}

bool IDevice9::ChooseAndCheckMultisample()
{
	if(this->d3d == NULL)
		return false;
	DWORD quality;
	if(this->d3d->CheckDeviceMultiSampleType(this->adapter,D3DDEVTYPE_HAL,D3DFMT_X8R8G8B8,false,D3DMULTISAMPLE_16_SAMPLES,&quality) == D3D_OK)
	{
		this->d3dPPFullScreen.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;
		this->d3dPPFullScreen.MultiSampleQuality = (quality > 0) ? quality - 1 : 0;
	}else if(this->d3d->CheckDeviceMultiSampleType(this->adapter,D3DDEVTYPE_HAL,D3DFMT_X8R8G8B8,false,D3DMULTISAMPLE_8_SAMPLES,&quality) == D3D_OK)
	{
		this->d3dPPFullScreen.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
		this->d3dPPFullScreen.MultiSampleQuality = (quality > 0) ? quality - 1 : 0;
	}else if(this->d3d->CheckDeviceMultiSampleType(this->adapter,D3DDEVTYPE_HAL,D3DFMT_X8R8G8B8,false,D3DMULTISAMPLE_4_SAMPLES,&quality) == D3D_OK)
	{
		this->d3dPPFullScreen.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
		this->d3dPPFullScreen.MultiSampleQuality = (quality > 0) ? quality - 1 : 0;
	}else if(this->d3d->CheckDeviceMultiSampleType(this->adapter,D3DDEVTYPE_HAL,D3DFMT_X8R8G8B8,false,D3DMULTISAMPLE_2_SAMPLES,&quality) == D3D_OK)
	{
		this->d3dPPFullScreen.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
		this->d3dPPFullScreen.MultiSampleQuality = (quality > 0) ? quality - 1 : 0;
	}else{
		this->d3dPPFullScreen.MultiSampleType = D3DMULTISAMPLE_NONE;
		this->d3dPPFullScreen.MultiSampleQuality = 0;
	}

	if(this->d3d->CheckDeviceMultiSampleType(this->adapter,D3DDEVTYPE_HAL,D3DFMT_X8R8G8B8,true,D3DMULTISAMPLE_16_SAMPLES,&quality) == D3D_OK)
	{
		this->d3dPPWindowed.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;
		this->d3dPPWindowed.MultiSampleQuality = (quality > 0) ? quality - 1 : 0;
	}else if(this->d3d->CheckDeviceMultiSampleType(this->adapter,D3DDEVTYPE_HAL,D3DFMT_X8R8G8B8,true,D3DMULTISAMPLE_8_SAMPLES,&quality) == D3D_OK)
	{
		this->d3dPPWindowed.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
		this->d3dPPWindowed.MultiSampleQuality = (quality > 0) ? quality - 1 : 0;
	}else if(this->d3d->CheckDeviceMultiSampleType(this->adapter,D3DDEVTYPE_HAL,D3DFMT_X8R8G8B8,true,D3DMULTISAMPLE_4_SAMPLES,&quality) == D3D_OK)
	{
		this->d3dPPWindowed.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
		this->d3dPPWindowed.MultiSampleQuality = (quality > 0) ? quality - 1 : 0;
	}else if(this->d3d->CheckDeviceMultiSampleType(this->adapter,D3DDEVTYPE_HAL,D3DFMT_X8R8G8B8,true,D3DMULTISAMPLE_2_SAMPLES,&quality) == D3D_OK)
	{
		this->d3dPPWindowed.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
		this->d3dPPWindowed.MultiSampleQuality = (quality > 0) ? quality - 1 : 0;
	}else{
		this->d3dPPWindowed.MultiSampleType = D3DMULTISAMPLE_NONE;
		this->d3dPPWindowed.MultiSampleQuality = 0;
	}
	return true;
}

bool IDevice9::CheckDevCaps()
{
	if(this->d3d == NULL)
		return false;
	if(FAILED(this->d3d->GetDeviceCaps(this->adapter,D3DDEVTYPE_HAL,&this->deviceCaps)))
		return false;
	if(!(this->deviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT))
		return false;
	if(!(this->deviceCaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE))
		return false;
	return true;
}

bool IDevice9::CreateDirectXDevice(bool bWindowed)
{
	if(this->d3d == NULL)
		return false;
	if(FAILED(this->d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,this->hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,bWindowed ? &this->d3dPPWindowed : &this->d3dPPFullScreen,&this->d3dDevice)))
	{
		eLOG(L"FAILED to create DirectX Device");
		return false;
	}
	else
	{
		LOG(L"DirectX Device was successfully created");
		return true;
	}
}