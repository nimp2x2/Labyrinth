#include "EngineHeaders.h"
#include <malloc.h>

EngineHeaders engineHeaders;

EngineHeaders::EngineHeaders()
{
	this->window = NULL;
	this->input = NULL;
	this->device = NULL;
	this->scene2d = NULL;
	this->resourceManager = NULL;
	this->camera = NULL;
}

EngineHeaders::~EngineHeaders()
{
	SQUALL_Sample_UnloadAll();
	SQUALL_Free();
	SAFE_RELEASE(this->font);
	if (this->camera) {
		this->camera->~ICamera();
		_aligned_free(this->camera);
	}
	SAFE_RELEASE(this->resourceManager);
	if (this->scene) {
		this->scene->~ISceneManager();
		_aligned_free(this->scene);
	}
	if (this->scene2d) {
		this->scene2d->~ISceneManager2D();
		_aligned_free(this->scene2d);
	}
	SAFE_RELEASE(this->device);
	SAFE_RELEASE(this->input);
	SAFE_RELEASE(this->window);
}

LRESULT CALLBACK MsgProc(HWND HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(engineHeaders.window->IWindowsMessageDispatcher(msg,wParam,lParam) || engineHeaders.input->IInputMessageDispatcher(msg,wParam,lParam))
		return 0;
	switch( msg )
    {
	case WM_DESTROY:
		engineHeaders.window->SetHWND(NULL);
		PostQuitMessage(0);
		return 0;
	}
    return DefWindowProc(HWND, msg, wParam, lParam);
}

bool EngineHeaders::Init(HINSTANCE hInst)
{
	this->window = new IWindows(800, 600);
	if(!this->window->CreateWnd(hInst))
		return false;
	this->window->RenderFunction = NULL;
	this->window->LogicFunction = NULL;
	this->window ->SetFullscreenState(false);
	this->input = new IInput(this->window->GetWindowHandler());
	this->device = new IDevice9(this->window->GetWindowHandler(),hInst);
	if(!this->device->CreateDirectX())
		return false;
	if(!this->device->ChooseAdapter())
		return false;
	D3DFORMAT format = D3DFMT_X8R8G8B8;
	if(!this->device->CheckBackBufferFormat(&format,1))
		return false;
	int width[] = {800, 1024, 1280, 1366, 1600, 1920};
	int height[] = {600, 768, 1024, 768, 900, 1080};

	if(!this->device->ChooseAndCheckDisplayMode(width, height, 6))
		return false;
	this->window->SetSizePermission(true);
	this->window->SetWindowWidthAndHeight(this->device->d3dPPWindowed.BackBufferWidth, this->device->d3dPPWindowed.BackBufferHeight);
	this->window->SetSizePermission(false);
	this->window->SetWindowPosition((int)(GetSystemMetrics(SM_CXSCREEN)/2 - this->device->d3dPPWindowed.BackBufferWidth/2), 
		(int)(GetSystemMetrics(SM_CYSCREEN)/2 - this->device->d3dPPWindowed.BackBufferHeight/2));

	if(!this->device->ChooseAndCheckMultisample())
		return false;
	if(!this->device->CheckDevCaps())
		return false;
	if(!this->device->CreateDirectXDevice(true))
		return false;

	this->window->LogicFunction = EngineHeaders::Logic;
	this->window->RenderFunction = EngineHeaders::Render;
	
	void * ptr = _aligned_malloc(sizeof(ISceneManager2D), 16);
	this->scene2d = new (ptr) ISceneManager2D(this->device->GetDevice());

	if(FAILED(this->scene2d->CreateLinePrimitivesVertexBuffer(2048)))
		return false;
	if(FAILED(this->scene2d->CreatePointPrimitivesVertexBuffer(2048)))
		return false;
	if(FAILED(this->scene2d->CreateIndexBuffer(65536*2)))
		return false;
	if(FAILED(this->scene2d->CreateVertexBuffer(65536*2)))
		return false;

	currentWidth = this->window->GetFullscreenState() ? (float)this->device->d3dPPFullScreen.BackBufferWidth : (float)this->device->d3dPPWindowed.BackBufferWidth;
	currentHeight = this->window->GetFullscreenState() ? (float)this->device->d3dPPFullScreen.BackBufferHeight : (float)this->device->d3dPPWindowed.BackBufferHeight;

	this->scene2d->SetProjMat(currentWidth, currentHeight, 1.0f, 100.0f);
	this->scene2d->SetViewMat(D3DXVECTOR3(0.0f, 0.0f, -1.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	this->resourceManager = new IResourceManager(this->device->GetDevice());
	if(!this->resourceManager->LoadTexture(L"Data/Textures/boxTexture.dds"))
		return false;
	if(!this->resourceManager->LoadTexture(L"Data/Textures/arial32pt.dds"))
		return false;
	if(!this->resourceManager->LoadTexture(L"Data/Textures/wallDiffuse.dds"))
		return false;
	if(!this->resourceManager->LoadTexture(L"Data/Textures/mainMenuLogo.dds"))
		return false;
	if(!this->resourceManager->LoadTexture(L"Data/Textures/exitDiffuse.dds"))
		return false;

	font = new IFont();
	if(!font->AppendFontFile(L"Data/Misc/arial32pt.bin"))
		return false;
	font->SetScale(1.0f);
	font->SetTextureAtlas(this->resourceManager->GetTexture(L"Data/Textures/arial32pt.dds"));

	ptr = _aligned_malloc(sizeof(ISceneManager), 16);
	this->scene = new (ptr) ISceneManager(this->device->GetDevice());

	if(FAILED(this->scene->CreateStaticSimpleIndexBuffer(8192*2)))
		return false;
	if(FAILED(this->scene->CreateStaticSimpleVertexBuffer(8192*2)))
		return false;

	ptr = _aligned_malloc(sizeof(ICamera), 16);
	this->camera = new (ptr) ICamera();

	this->camera->SetEyeVector(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
	camera->SetProjectionMatrix(D3DX_PI / 4, currentWidth / currentHeight, 0.1f, 1000.0f);
	this->scene->SetProjMatrix(this->camera->GetProjMatrix());
	this->scene->SetViewMatrix(this->camera->GetViewMatrix());

	this->resourceManager->LoadSimpleMesh(L"Data/Meshes/wall.bmf",this->resourceManager->GetTexture(L"Data/Textures/wallDiffuse.dds"));
	this->resourceManager->LoadSimpleMesh(L"Data/Meshes/exit.bmf",this->resourceManager->GetTexture(L"Data/Textures/exitDiffuse.dds"));
	this->resourceManager->LoadSimpleMesh(L"Data/Meshes/emptywall.bmf",this->resourceManager->GetTexture(L"Data/Textures/wallDiffuse.dds"));
	D3DXMATRIXA16 *world1 = new D3DXMATRIXA16;
	D3DXMatrixIdentity(world1);
	D3DXMATRIXA16 *world2 = new D3DXMATRIXA16;
	D3DXMatrixIdentity(world2);
	D3DXMATRIXA16 *world3 = new D3DXMATRIXA16;
	D3DXMatrixIdentity(world3);
	engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/wall.bmf")->SetWorldMatrix(&SmartPointer<D3DXMATRIXA16>(world1));
	engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/emptywall.bmf")->SetWorldMatrix(&SmartPointer<D3DXMATRIXA16>(world2));
	engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/exit.bmf")->SetWorldMatrix(&SmartPointer<D3DXMATRIXA16>(world3));
	engineHeaders.scene->StartSimpleRenderQueueFill();
	engineHeaders.scene->AddStaticSimpleMesh(engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/wall.bmf"));	
	engineHeaders.scene->AddStaticSimpleMesh(engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/emptywall.bmf"));	
	engineHeaders.scene->AddStaticSimpleMesh(engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/exit.bmf"));
	engineHeaders.scene->EndSimpleRenderQueueFill();
	
	/*this->device->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	this->device->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);*/

	this->device->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	this->device->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2,D3DTA_TEXTURE);
	this->device->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	this->device->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->device->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->device->GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	this->device->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	this->device->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	this->device->GetDevice()->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, engineHeaders.device->deviceCaps.MaxAnisotropy);

	squall_parameters_t par;
	par.Channels = 4;
	par.Window = engineHeaders.window->GetWindowHandler();
	par.Device = 0;
	par.SampleRate = 44100;
	par.BitPerSample = 16;
	par.UseHW2D = true;
	par.UseHW3D = true;
	par.UseAlg = 0;
	par.BufferSize = 400;
	par.ListenerMode = 0;
	par.DistanceFactor = 1.0f;
	par.RolloffFactor = 1.0f;
	par.DopplerFactor = 1.0f;

	if (SQUALL_Init(&par) < 0)
	{
		return false;
	}

	this->mainMenuSound = SQUALL_Sample_LoadFile("Data/Sounds/welcome to silent hill.mp3", 1, 0);
	this->enviroment = SQUALL_Sample_LoadFile("Data/Sounds/whitenoise.mp3", 1, 0);

	engineHeaders.channel1 = SQUALL_Sample_Play(engineHeaders.mainMenuSound, 1, 0, 1);
	engineHeaders.channel2 = SQUALL_Sample_Play(engineHeaders.enviroment, 1, 0, 1);
	SQUALL_Channel_Pause(engineHeaders.channel1, 1);
	SQUALL_Channel_Pause(engineHeaders.channel2, 1);

	engineHeaders.AdjustHAndW();
	return true;
}

void EngineHeaders::Run()
{
	this->window->Run();
}

bool EngineHeaders::Logic()
{
	engineHeaders.input->ClearInput();
	return true;
}

bool EngineHeaders::Render()
{
	return true;
}

void EngineHeaders::OnLost()
{
	engineHeaders.scene2d->ReleaseAllBuffers();
	engineHeaders.scene->ReleaseAllBuffers();
}

void EngineHeaders::OnReset()
{
	engineHeaders.device->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	engineHeaders.device->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2,D3DTA_TEXTURE);
	engineHeaders.device->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	engineHeaders.device->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	engineHeaders.device->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	engineHeaders.device->GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	engineHeaders.device->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	engineHeaders.device->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	engineHeaders.device->GetDevice()->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, engineHeaders.device->deviceCaps.MaxAnisotropy);

	engineHeaders.scene2d->CreateLinePrimitivesVertexBuffer(2048);
	engineHeaders.scene2d->CreatePointPrimitivesVertexBuffer(2048);
	engineHeaders.scene2d->CreateIndexBuffer(65536*2);
	engineHeaders.scene2d->CreateVertexBuffer(65536*2);

	engineHeaders.scene->CreateStaticSimpleIndexBuffer(8192*2);
	engineHeaders.scene->CreateStaticSimpleVertexBuffer(8192*2);

	engineHeaders.scene->StartSimpleRenderQueueFill();
	engineHeaders.scene->AddStaticSimpleMesh(engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/wall.bmf"));	
	engineHeaders.scene->AddStaticSimpleMesh(engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/emptywall.bmf"));	
	engineHeaders.scene->AddStaticSimpleMesh(engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/exit.bmf"));
	engineHeaders.scene->EndSimpleRenderQueueFill();
}

void EngineHeaders::AdjustHAndW()
{
	switch((int)engineHeaders.currentWidth)
	{
	case 800:
		engineHeaders.font->SetScale(0.8f);
		engineHeaders.maxXBlocks = 50;
		engineHeaders.maxYBlocks = 37;
		break;
	case 1024:
		engineHeaders.font->SetScale(1.0f);
		engineHeaders.maxXBlocks = 64;
		engineHeaders.maxYBlocks = 48;
		break;
	case 1280:
		engineHeaders.font->SetScale(1.1f);
		engineHeaders.maxXBlocks = 80;
		engineHeaders.maxYBlocks = 64;
		break;
	case 1366:
		engineHeaders.font->SetScale(1.4f);
		engineHeaders.maxXBlocks = 85;
		engineHeaders.maxYBlocks = 48;
		break;
	case 1600:
		engineHeaders.font->SetScale(1.6f);
		engineHeaders.maxXBlocks = 100;
		engineHeaders.maxYBlocks = 56;
		break;
	case 1920:
		engineHeaders.font->SetScale(1.8f);
		engineHeaders.maxXBlocks = 120;
		engineHeaders.maxYBlocks = 67;
		break;
	}
}