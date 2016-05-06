#ifndef ENGINE_HEADERS_H
#define ENGINE_HEADERS_H

#include "IWindows.h"
#include "IInput.h"
#include "IDevice9.h"
#include "ISceneManager2D.h"
#include "IResourceManager.h"
#include "ISceneManager.h"
#include "ICamera.h"

class EngineHeaders
{
public:
	IWindows				*window;
	IInput					*input;
	IDevice9				*device;
	ISceneManager2D			*scene2d;
	ISceneManager			*scene;
	IResourceManager		*resourceManager;
	ICamera					*camera;
	IFont					*font;
	float					currentWidth;
	float					currentHeight;
	int						mainMenuSound;
	int						footsteps;
	int						enviroment;
	int						channel1;
	int						channel2;
	void					OnLost();
	void					OnReset();
	//width and height adjusment
	UINT					maxXBlocks;
	UINT					maxYBlocks;
	void					AdjustHAndW();

	static bool Logic();
	static bool Render();

	bool					Init(HINSTANCE hInst);
	void					Run();
	EngineHeaders();
	~EngineHeaders();
};

extern EngineHeaders engineHeaders;

#endif