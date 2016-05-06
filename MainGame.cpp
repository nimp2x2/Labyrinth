#include "MainGame.h"

MainGame::MainGame()
{
	this->pathFinded = false;
	this->showPath = false;
	this->pathExist = true;
	this->mainGameState = 0; // 0 - read data, 1 - main screen, 2 - map, 3 - failed to read from data.bin, 4 - victory screen, 5 - file not found
	this->currentPosX = 0;
	this->currentPosY = 0;
	this->offsetx = 0;
	this->offsety = 0;
	this->xmax = 0;
	this->ymax = 0;
	this->xadd = 0;
	this->yadd = 0;
	this->icounter = 0;
	this->jcounter = 0;
	this->dx = 0;
	this->dy = 0;
	this->oldx = 0;
	this->oldy = 0;
	this->drag = false;
	this->startX = 0;
	this->startY = 0;
	this->emptyBlock = new ISprite(16.0f, 16.0f, 2.0f, D3DCOLOR_XRGB(255,255,255), engineHeaders.resourceManager->GetTexture(L"Data/Textures/boxTexture.dds"));
	this->emptyBlock->SetTexturePosition(0.0f, 0.0f, 15.0f, 15.0f);
	this->wall = new ISprite(16.0f, 16.0f, 2.0f, D3DCOLOR_XRGB(255,255,255), engineHeaders.resourceManager->GetTexture(L"Data/Textures/boxTexture.dds"));
	this->wall->SetTexturePosition(32.0f, 0.0f, 47.0f, 15.0f);
	this->startPoint = new ISprite(16.0f, 16.0f, 2.0f, D3DCOLOR_XRGB(255,255,255), engineHeaders.resourceManager->GetTexture(L"Data/Textures/boxTexture.dds"));
	this->startPoint->SetTexturePosition(16.0f, 0.0f, 31.0f, 15.0f);
	this->finishPoint = new ISprite(16.0f, 16.0f, 2.0f, D3DCOLOR_XRGB(255,255,255), engineHeaders.resourceManager->GetTexture(L"Data/Textures/boxTexture.dds"));
	this->finishPoint->SetTexturePosition(48.0f, 0.0f, 63.0f, 15.0f);
	this->upArrow = new ISprite(16.0f, 16.0f, 2.0f, D3DCOLOR_XRGB(255,255,255), engineHeaders.resourceManager->GetTexture(L"Data/Textures/boxTexture.dds"));
	this->upArrow->SetTexturePosition(0.0f, 16.0f, 15.0f, 31.0f);
	this->downArrow = new ISprite(16.0f, 16.0f, 2.0f, D3DCOLOR_XRGB(255,255,255), engineHeaders.resourceManager->GetTexture(L"Data/Textures/boxTexture.dds"));
	this->downArrow->SetTexturePosition(32.0f, 16.0f, 47.0f, 31.0f);
	this->rightArrow = new ISprite(16.0f, 16.0f, 2.0f, D3DCOLOR_XRGB(255,255,255), engineHeaders.resourceManager->GetTexture(L"Data/Textures/boxTexture.dds"));
	this->rightArrow->SetTexturePosition(16.0f, 16.0f, 31.0f, 31.0f);
	this->leftArrow = new ISprite(16.0f, 16.0f, 2.0f, D3DCOLOR_XRGB(255,255,255), engineHeaders.resourceManager->GetTexture(L"Data/Textures/boxTexture.dds"));
	this->leftArrow->SetTexturePosition(48.0f, 16.0f, 63.0f, 31.0f);
	this->undiscovered = new ISprite(16.0f, 16.0f, 2.0f, D3DCOLOR_XRGB(255,255,255), engineHeaders.resourceManager->GetTexture(L"Data/Textures/boxTexture.dds"));
	this->undiscovered->SetTexturePosition(0.0f, 32.0f, 15.0f, 47.0f);
	this->mapWidth = 10;
	this->mapHeight = 10;
	this->map = NULL;
	this->map2 = NULL;
	this->mouseClip = false;
	this->camx = 0;
	this->camOldx = 0;
	this->camy = 0;
	this->camOldy = 0;
	this->recover = false;
	this->filename = NULL;
	this->filenamepos = 0;
	this->dest.clear();
	this->currentExit = 0;
}

MainGame::~MainGame()
{
	if(this->undiscovered != NULL)
		delete this->undiscovered;
	if(this->emptyBlock != NULL)
		delete this->emptyBlock;
	if(this->wall != NULL)
		delete this->wall;
	if(this->finishPoint != NULL)
		delete this->finishPoint;
	if(this->startPoint != NULL)
		delete this->startPoint;
	if(this->leftArrow != NULL)
		delete this->leftArrow;
	if(this->upArrow != NULL)
		delete this->upArrow;
	if(this->downArrow != NULL)
		delete this->downArrow;
	if(this->rightArrow != NULL)
		delete this->rightArrow;
	if(this->map != NULL)
	{
		for(UINT i = 0; i < this->mapWidth; i++)
			delete []map[i];
		delete []map;
	}
	if(this->map2 != NULL)
	{
		for(UINT i = 0; i < this->mapWidth; i++)
			delete []map2[i];
		delete []map2;
	}
	if(this->filename != NULL)
		delete []this->filename;
}

bool MainGame::Render(MainGame *mgame)
{
	float start = 100.0f;
	float end = 180;
	switch(mgame->mainGameState)
	{
	case 0:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, (engineHeaders.currentHeight/2+engineHeaders.font->GetScale()*32.0f), C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Введите имя файла(backspace стереть)");
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"%s", mgame->filename);
		engineHeaders.scene2d->sceneEnd();
		break;
	case 1:
		engineHeaders.device->GetDevice()->SetRenderState(D3DRS_FOGENABLE,true);
		engineHeaders.device->GetDevice()->SetRenderState(D3DRS_RANGEFOGENABLE,true);
		engineHeaders.device->GetDevice()->SetRenderState(D3DRS_FOGCOLOR,D3DCOLOR_XRGB(128,128,128));
		engineHeaders.device->GetDevice()->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
		engineHeaders.device->GetDevice()->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&start));
		engineHeaders.device->GetDevice()->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&end));
		for(int i = -6; i < 7; i++)
			for( int j = -6; j < 7; j++)
			{
				if(j == -6 || j == 6 || i == -6 || i ==6 )
				{
					engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/wall.bmf")->SetRenderable(true);
					engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/emptywall.bmf")->SetRenderable(false);
					engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/exit.bmf")->SetRenderable(false);
					D3DXMatrixTranslation(engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/wall.bmf")->GetWorldMatrix()->GetData(), (float)i*30,0, (float)j*30);
					engineHeaders.scene->SceneBegin();
					engineHeaders.scene->SceneEnd();
				}else if(mgame->startX + i >= 0 && mgame->startX + i < mgame->mapWidth &&
					mgame->startY + j >= 0 && mgame->startY + j < mgame->mapHeight)
				{
					if(mgame->map[mgame->startX + i][mgame->startY + j] == 1){
						engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/wall.bmf")->SetRenderable(true);
						engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/emptywall.bmf")->SetRenderable(false);
						engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/exit.bmf")->SetRenderable(false);
						D3DXMatrixTranslation(engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/wall.bmf")->GetWorldMatrix()->GetData(), (float)i*30,0, (float)j*30);
						engineHeaders.scene->SceneBegin();
						engineHeaders.scene->SceneEnd();
					}else if((mgame->map[mgame->startX + i][mgame->startY + j] == 2)){
						engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/wall.bmf")->SetRenderable(false);
						engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/emptywall.bmf")->SetRenderable(false);
						engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/exit.bmf")->SetRenderable(true);
						D3DXMatrixTranslation(engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/exit.bmf")->GetWorldMatrix()->GetData(), (float)i*30, 0.0f, (float)j*30);
						engineHeaders.scene->SceneBegin();
						engineHeaders.scene->SceneEnd();
					}else{
						engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/wall.bmf")->SetRenderable(false);
						engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/emptywall.bmf")->SetRenderable(true);
						engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/exit.bmf")->SetRenderable(false);
						D3DXMatrixTranslation(engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/emptywall.bmf")->GetWorldMatrix()->GetData(), (float)i*30,0, (float)j*30);
						engineHeaders.scene->SceneBegin();
						engineHeaders.scene->SceneEnd();
					}

				}else if(mgame->startX + i == -1 || mgame->startX + i == mgame->mapWidth ||
						mgame->startY + j == -1 || mgame->startY + j == mgame->mapHeight)
				{
					engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/wall.bmf")->SetRenderable(true);
					engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/emptywall.bmf")->SetRenderable(false);
					D3DXMatrixTranslation(engineHeaders.resourceManager->GetSimpleMesh(L"Data/Meshes/wall.bmf")->GetWorldMatrix()->GetData(), (float)i*30, 0, (float)j*30);
					engineHeaders.scene->SceneBegin();
					engineHeaders.scene->SceneEnd();
				}
			}
		engineHeaders.device->GetDevice()->SetRenderState(D3DRS_FOGENABLE,false);
		engineHeaders.device->GetDevice()->SetRenderState(D3DRS_RANGEFOGENABLE,false);
		break;
	case 2:
		engineHeaders.scene2d->sceneBegin();
		//debug info 
		//engineHeaders.scene2d->DrawSimpleText(engineHeaders.font, 2.0f, 600.0f,  0, 256, L"offsetx = %d, offsety = %d\n currentX = %u, currentY = %u",mgame->offsetx,mgame->offsety,mgame->currentPosX, mgame->currentPosY);
		if(mgame->mapWidth > engineHeaders.maxXBlocks || mgame->mapHeight > engineHeaders.maxYBlocks)
		{
			mgame->icounter = 0;
			mgame->jcounter = 0;
			mgame->xadd = 0;
			mgame->xmax = engineHeaders.maxXBlocks < mgame->mapWidth ? engineHeaders.maxXBlocks : mgame->mapWidth;
			mgame->ymax = engineHeaders.maxYBlocks < mgame->mapHeight ? engineHeaders.maxYBlocks : mgame->mapHeight;
			//xmax and iset setting
			if(mgame->offsetx > 0 && mgame->currentPosX != mgame->mapWidth - engineHeaders.maxXBlocks)
			{
				mgame->xmax = engineHeaders.maxXBlocks < mgame->mapWidth ? engineHeaders.maxXBlocks+1 : mgame->mapWidth;
			}else if(mgame->offsetx < 0  && mgame->currentPosX != 0)
			{
				mgame->xmax = engineHeaders.maxXBlocks < mgame->mapWidth ? engineHeaders.maxXBlocks : mgame->mapWidth;
				mgame->xadd = -1;
				mgame->icounter = -1;
			}
			//ymax setting
			if(mgame->offsety < 0 && mgame->currentPosY != 0)
			{
				mgame->ymax = engineHeaders.maxYBlocks < mgame->mapHeight ? engineHeaders.maxYBlocks : mgame->mapHeight;
				mgame->jcounter = -1;
			}else if(mgame->offsety > 0 && mgame->currentPosY != mgame->mapHeight - engineHeaders.maxYBlocks)
			{
				mgame->ymax = engineHeaders.maxYBlocks < mgame->mapHeight ? engineHeaders.maxYBlocks+1 : mgame->mapHeight;
			}
			//main drawing cycle
			while(mgame->jcounter < mgame->ymax)
			{
				while(mgame->icounter < mgame->xmax)
				{
					if(mgame->map[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter] == 0)
						if(mgame->map2[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter])
							engineHeaders.scene2d->DrawSprite(mgame->emptyBlock, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
						else
							engineHeaders.scene2d->DrawSprite(mgame->undiscovered, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
					else if(mgame->map[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter] == 1)
						if(mgame->map2[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter])
							engineHeaders.scene2d->DrawSprite(mgame->wall, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
						else
							engineHeaders.scene2d->DrawSprite(mgame->undiscovered, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
					else if(mgame->map[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter] == 2)
						if(mgame->map2[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter])
							engineHeaders.scene2d->DrawSprite(mgame->finishPoint, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
						else
							engineHeaders.scene2d->DrawSprite(mgame->undiscovered, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
					else if(mgame->map[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter] == 3)
						if(mgame->map2[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter])
							engineHeaders.scene2d->DrawSprite(mgame->startPoint, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
						else
							engineHeaders.scene2d->DrawSprite(mgame->undiscovered, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
					else if(mgame->pathExist && mgame->showPath)
					{
						if(mgame->map[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter] == 4)
							engineHeaders.scene2d->DrawSprite(mgame->upArrow, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
						else if(mgame->map[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter] == 5)
							engineHeaders.scene2d->DrawSprite(mgame->downArrow, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
						else if(mgame->map[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter] == 6)
							engineHeaders.scene2d->DrawSprite(mgame->rightArrow, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
						else if(mgame->map[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter] == 7)
							engineHeaders.scene2d->DrawSprite(mgame->leftArrow, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
					}else
						if(mgame->map2[mgame->currentPosX+mgame->icounter][mgame->currentPosY+mgame->jcounter])
							engineHeaders.scene2d->DrawSprite(mgame->emptyBlock, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
						else
							engineHeaders.scene2d->DrawSprite(mgame->undiscovered, (float)(mgame->icounter*16-mgame->offsetx), (float)(mgame->jcounter*16-mgame->offsety+16));
					mgame->icounter+=1;
				}
				mgame->icounter = mgame->xadd;
				mgame->jcounter += 1;
			}
		}
		else{
			for(UINT jcounter = 0; jcounter < mgame->mapHeight; jcounter++)
				for(UINT icounter = 0; icounter < mgame->mapWidth; icounter++)
				{
					if(mgame->map[icounter][jcounter] == 0)
						if(mgame->map2[icounter][jcounter])
							engineHeaders.scene2d->DrawSprite(mgame->emptyBlock,(float)(icounter*16),(float)(jcounter*16)+16);
						else
							engineHeaders.scene2d->DrawSprite(mgame->undiscovered,(float)(icounter*16),(float)(jcounter*16)+16);
					else if(mgame->map[icounter][jcounter] == 1)
						if(mgame->map2[icounter][jcounter])
							engineHeaders.scene2d->DrawSprite(mgame->wall,(float)(icounter*16),(float)(jcounter*16)+16);
						else
							engineHeaders.scene2d->DrawSprite(mgame->undiscovered,(float)(icounter*16),(float)(jcounter*16)+16);
					else if(mgame->map[icounter][jcounter] == 2)
						if(mgame->map2[icounter][jcounter])
							engineHeaders.scene2d->DrawSprite(mgame->finishPoint,(float)(icounter*16),(float)(jcounter*16)+16);
						else
							engineHeaders.scene2d->DrawSprite(mgame->undiscovered,(float)(icounter*16),(float)(jcounter*16)+16);
					else if(mgame->map[icounter][jcounter] == 3)
						if(mgame->map2[icounter][jcounter])
							engineHeaders.scene2d->DrawSprite(mgame->startPoint,(float)(icounter*16),(float)(jcounter*16)+16);
						else
							engineHeaders.scene2d->DrawSprite(mgame->undiscovered,(float)(icounter*16),(float)(jcounter*16)+16);
					else if(mgame->showPath && mgame->pathExist)
					{
						if(mgame->map[icounter][jcounter] == 4)
						engineHeaders.scene2d->DrawSprite(mgame->upArrow,(float)(icounter*16),(float)(jcounter*16)+16);
						else if(mgame->map[icounter][jcounter] == 5)
							engineHeaders.scene2d->DrawSprite(mgame->downArrow,(float)(icounter*16),(float)(jcounter*16)+16);
						else if(mgame->map[icounter][jcounter] == 6)
							engineHeaders.scene2d->DrawSprite(mgame->rightArrow,(float)(icounter*16),(float)(jcounter*16)+16);
						else if(mgame->map[icounter][jcounter] == 7)
							engineHeaders.scene2d->DrawSprite(mgame->leftArrow,(float)(icounter*16),(float)(jcounter*16)+16);
					}else
						if(mgame->map2[icounter][jcounter])
							engineHeaders.scene2d->DrawSprite(mgame->emptyBlock,(float)(icounter*16),(float)(jcounter*16)+16);
						else
							engineHeaders.scene2d->DrawSprite(mgame->undiscovered,(float)(icounter*16),(float)(jcounter*16)+16);
				}
		}
		if(mgame->showPath && !mgame->pathExist)
		{
			engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Не удалось найти выход");
		}
		engineHeaders.scene2d->sceneEnd();
		break;
	case 3:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Не удалось открыть файл %s\nНажмите ESCAPE для выхода", mgame->filename);
		engineHeaders.scene2d->sceneEnd();
		break;
	case 4:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Вы нашли выход\nНажмите ESCAPE для выхода");
		engineHeaders.scene2d->sceneEnd();
		break;
	case 6:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Файл не найден.");
		engineHeaders.scene2d->sceneEnd();
		break;
	}
	return true;
}

DWORD MainGame::Logic(MainGame *mgame)
{
	wchar_t buf;
	if(engineHeaders.input->GetCurrentKeyState(VK_ESCAPE))
	{
		if(mgame->map != NULL)
		{
			for(UINT i = 0; i < mgame->mapWidth; i++)
				delete []mgame->map[i];
			delete []mgame->map;
		}
		if(mgame->map2 != NULL)
		{
			for(UINT i = 0; i < mgame->mapWidth; i++)
				delete []mgame->map2[i];
			delete []mgame->map2;
		}
		mgame->map = NULL;
		mgame->map2 = NULL;
		mgame->mainGameState = 0;
		return 2;
	}
	switch(mgame->mainGameState)
	{
		case 0:
			if(engineHeaders.input->GetKeyPressNumber(VK_BACK) > 0)
			{
				if(mgame->filenamepos > 0)
				{
					mgame->filename[mgame->filenamepos] = 0;
					mgame->filename[mgame->filenamepos-1] = 0;
					mgame->filenamepos--;
				}
			}
			buf = engineHeaders.input->GetUnicodeFromKeyQueue();
			if(buf != NULL && ( (buf >= 0x41 && buf <= 0x5A) || (buf >= 0x61 && buf <= 0x7A) || (buf == 0x2E) || (buf >= 0x30 && buf <= 0x39) ) )
			{
				if(mgame->filenamepos < 255)
				{
					mgame->filename[mgame->filenamepos] = buf;
					mgame->filename[mgame->filenamepos+1] = 0;
					mgame->filenamepos++;
				}
			}
			if(engineHeaders.input->GetKeyPressNumber(VK_RETURN) > 0)
			{
				if(mgame->filenamepos == 0)
				{
					mgame->mainGameState = 6;
					return 1;
				}
				if(GetFileAttributes(mgame->filename) == DWORD(-1))
				{
					mgame->mainGameState= 6;
					return 1;
				}
				FILE *file;
				_wfopen_s(&file,mgame->filename,L"rb");
				if(file != NULL)
				{
					fread(&mgame->mapWidth,sizeof(UINT),1,file);
					fread(&mgame->mapHeight,sizeof(UINT),1,file);
					fread(&mgame->startX,sizeof(UINT),1,file);
					fread(&mgame->startY,sizeof(UINT),1,file);
					UINT size;
					fread(&size,sizeof(UINT),1,file);
					mgame->dest.resize(size);
					for(UINT i = 0; i < size; i++)
						fread(&mgame->dest[i],sizeof(UINT),1,file);
					if(mgame->map != NULL)
					{
						for(UINT i = 0; i < mgame->mapWidth; i++)
							delete []mgame->map[i];
						delete []mgame->map;
						mgame->map = NULL;
					}
					if(mgame->map2 != NULL)
					{
						for(UINT i = 0; i < mgame->mapWidth; i++)
							delete []mgame->map2[i];
						delete []mgame->map2;
						mgame->map2 = NULL;
					}
					mgame->map = new UINT*[mgame->mapWidth];
					for(UINT i = 0; i < mgame->mapWidth; i++)
					{
						mgame->map[i] = new UINT[mgame->mapHeight];
						for(UINT j = 0; j < mgame->mapHeight; j++)
							mgame->map[i][j] = 0;
					}
					for(UINT i = 0; i < mgame->mapWidth; i++)
						for(UINT j = 0; j < mgame->mapHeight; j++)
							fread(&mgame->map[i][j],sizeof(UINT),1,file);
					mgame->map2 = new bool*[mgame->mapWidth];
					for(UINT i = 0; i < mgame->mapWidth; i++)
					{
						mgame->map2[i] = new bool[mgame->mapHeight];
						for(UINT j = 0; j < mgame->mapHeight; j++)
							mgame->map2[i][j] = false;
					}
					mgame->map2[mgame->startX][mgame->startY] = true;
					if(mgame->startX != 0)
					{
						mgame->map2[mgame->startX-1][mgame->startY] = true;
						if(mgame->startY != 0)
							mgame->map2[mgame->startX-1][mgame->startY-1] = true;
						if(mgame->startY +1 < mgame->mapHeight)
							mgame->map2[mgame->startX-1][mgame->startY+1] = true;
					}
					if(mgame->startX+1 < mgame->mapWidth)
					{
						mgame->map2[mgame->startX+1][mgame->startY] = true;
						if(mgame->startY != 0)
							mgame->map2[mgame->startX+1][mgame->startY-1] = true;
						if(mgame->startY+1 < mgame->mapHeight)
							mgame->map2[mgame->startX+1][mgame->startY+1] = true;
					}
					if(mgame->startY != 0)
						mgame->map2[mgame->startX][mgame->startY-1] = true;
					if(mgame->startY+1 < mgame->mapHeight)
						mgame->map2[mgame->startX][mgame->startY+1] = true;

					mgame->mainGameState = 1;
					fclose(file);
					mgame->pathFinded = false;
					mgame->showPath = false;
					mgame->pathExist = true;
					mgame->drag = false;
					mgame->mouseClip = false;
					mgame->recover = false;
					engineHeaders.camera->Reset();
					engineHeaders.camera->eye.x = 0.0f;
					engineHeaders.camera->eye.y = 20.0f;
					engineHeaders.camera->eye.z = 0.0f;
				}else{
					if(mgame->map != NULL)
					{
						for(UINT i = 0; i < mgame->mapWidth; i++)
							delete []mgame->map[i];
						delete []mgame->map;
						mgame->map = NULL;
					}
					if(mgame->map2 != NULL)
					{
						for(UINT i = 0; i < mgame->mapWidth; i++)
							delete []mgame->map2[i];
						delete []mgame->map2;
						mgame->map2 = NULL;
					}
					mgame->mainGameState = 3;
				}
			}
			break;
		case 1:
			if(engineHeaders.input->GetKeyPressNumber(0x4D) > 0)
			{
				mgame->drag = false;
				if(mgame->startX > engineHeaders.maxXBlocks)
					mgame->currentPosX = mgame->startX - (UINT)(engineHeaders.maxXBlocks / 2);
				else
					mgame->currentPosX = 0;
				if(mgame->startY > engineHeaders.maxYBlocks)
					mgame->currentPosY = mgame->startY - (UINT)(engineHeaders.maxYBlocks / 2);
				else
					mgame->currentPosY = 0;

				mgame->offsetx = mgame->offsety = 0;
				mgame->mainGameState = 2;
			}
			if(mgame->mouseClip)
			{
				if(engineHeaders.input->GetMouseX() > (int)(engineHeaders.currentWidth/2+100) 
					|| engineHeaders.input->GetMouseX() < (int)(engineHeaders.currentWidth/2-100) 
					|| engineHeaders.input->GetMouseY() > (int)(engineHeaders.currentHeight/2+100) 
					|| engineHeaders.input->GetMouseY() < (int)(engineHeaders.currentHeight/2-100))
				{
					SetCursorPos((int)(engineHeaders.window->GetWindowX()+engineHeaders.currentWidth/2), (int)(engineHeaders.window->GetWindowY()+engineHeaders.currentHeight/2));
					mgame->camOldx = mgame->camx = engineHeaders.input->GetMouseX();
					mgame->camOldy = mgame->camy = engineHeaders.input->GetMouseY();
					mgame->recover = true;
				}
				else
				{
					mgame->camOldx = mgame->camx;
					mgame->camx = engineHeaders.input->GetMouseX();
					mgame->camOldy = mgame->camy;
					mgame->camy = engineHeaders.input->GetMouseY();
					if(!mgame->recover)
					{
						engineHeaders.camera->Yaw((mgame->camOldx  - mgame->camx) / 5.0f);
						engineHeaders.camera->Pitch((-mgame->camOldy  + mgame->camy) / 5.0f);
						engineHeaders.camera->eye.y = 20.0f;
					}
					else
						mgame->recover = false;
			
				}
			}
			if(engineHeaders.input->GetKeyPressNumber(VK_TAB) > 0)
			{
				if(mgame->mouseClip)
				{
					mgame->mouseClip = false;
				}
				else
				{
					mgame->camx = engineHeaders.input->GetMouseX();
					mgame->camy = engineHeaders.input->GetMouseY();
					mgame->mouseClip = true;
				}
			}
			if(engineHeaders.input->GetCurrentKeyState(VK_LEFT) || engineHeaders.input->GetCurrentKeyState(0x41))
			{
				engineHeaders.camera->Move(-0.5f,0.0f,0.0f);
				mgame->PerformCollisionDetection();
			}
			if(engineHeaders.input->GetCurrentKeyState(VK_RIGHT) || engineHeaders.input->GetCurrentKeyState(0x44))
			{
				engineHeaders.camera->Move(0.5f,0.0f,0.0f);
				mgame->PerformCollisionDetection();
			}
			if(engineHeaders.input->GetCurrentKeyState(VK_UP) || engineHeaders.input->GetCurrentKeyState(0x57))
			{
				engineHeaders.camera->Move(0.0f,0.0f,0.5f);
				mgame->PerformCollisionDetection();
			}
			if(engineHeaders.input->GetCurrentKeyState(VK_DOWN) || engineHeaders.input->GetCurrentKeyState(0x53))
			{
				engineHeaders.camera->Move(0.0f,0.0f,-0.5f);
				mgame->PerformCollisionDetection();
			}
			engineHeaders.scene->SetViewMatrix(engineHeaders.camera->UpdateViewMatrix());
			engineHeaders.input->ClearInput();
			break;
		case 2:
			if(mgame->mapWidth > engineHeaders.maxXBlocks || mgame->mapHeight > engineHeaders.maxYBlocks)
			{
				if(engineHeaders.input->GetKeyPressNumber(VK_RBUTTON))
				{
					mgame->oldx = engineHeaders.input->GetMouseX();
					mgame->oldy = engineHeaders.input->GetMouseY();
					mgame->dx = 0;
					mgame->dy = 0;
					mgame->drag = true;
				}
				if(engineHeaders.input->GetCurrentKeyState(VK_RBUTTON) && mgame->drag)
				{
					//x
					if(mgame->mapWidth > (engineHeaders.maxXBlocks))
					{
						mgame->dx = -engineHeaders.input->GetMouseX() + mgame->oldx;
						mgame->offsetx += mgame->dx;
					}
					else
					{
						mgame->dx = 0;
					}
					if(mgame->offsetx > 0 && mgame->currentPosX == (mgame->mapWidth-(engineHeaders.maxXBlocks)))
						mgame->offsetx = 0;
					if(mgame->offsetx < 0 && mgame->currentPosX == 0)
						mgame->offsetx = 0;
					if(mgame->offsetx >= 16)
					{
						while(mgame->offsetx >= 16)
						{
							mgame->offsetx -= 16;
							if(mgame->currentPosX < (mgame->mapWidth-(engineHeaders.maxXBlocks)))
								mgame->currentPosX++;
							else
								mgame->offsetx = 0;
					
						}
					}
					if(mgame->offsetx <= -16)
					{
						while(mgame->offsetx <= -16)
						{
							mgame->offsetx += 16;
							if(mgame->currentPosX > 0)
								mgame->currentPosX--;
							else
								mgame->offsetx = 0;
						}
					}
					mgame->oldx = (UINT)engineHeaders.input->GetMouseX();
					//y
					if(mgame->mapHeight > (engineHeaders.maxYBlocks))
					{
						mgame->dy = engineHeaders.input->GetMouseY() - mgame->oldy;
						mgame->offsety -= mgame->dy;
					}
					else
						mgame->dy = 0;

					if(mgame->offsety > 0 && mgame->currentPosY == (mgame->mapHeight-(engineHeaders.maxYBlocks)))
						mgame->offsety = 0;
					if(mgame->offsety < 0 && mgame->currentPosY == 0)
						mgame->offsety = 0;
					if(mgame->offsety >= 16)
					{
						while(mgame->offsety >= 16)
						{
							mgame->offsety -= 16;
							if(mgame->currentPosY < (mgame->mapHeight-(engineHeaders.maxYBlocks)))
								mgame->currentPosY++;
							else
								mgame->offsety = 0;
					
						}
					}
					if(mgame->offsety <= -16)
					{
						while(mgame->offsety <= -16)
						{
							mgame->offsety += 16;
							if(mgame->currentPosY > 0)
								mgame->currentPosY--;
							else
								mgame->offsety = 0;
						}
					}
					mgame->oldy = (UINT)engineHeaders.input->GetMouseY();
				}else
				{
					mgame->drag = false;
					mgame->dx = 0;
					mgame->dy = 0;
				}
			}
			if(engineHeaders.input->GetKeyPressNumber(0x4D) > 0)
			{
				mgame->mainGameState = 1;
				mgame->showPath = false;
			}
			if(engineHeaders.input->GetKeyPressNumber(0x4E) > 0)
			{
				if(!mgame->showPath)
				{
					UINT destX,destY;
					destX = mgame->dest[mgame->currentExit*2];
					destY = mgame->dest[mgame->currentExit*2+1];
					if(mgame->startX != 0)
						if(destX == mgame->startX-1 && destY == mgame->startY)
						{
							mgame->currentExit++;
							if(mgame->currentExit*2 > mgame->dest.size()-1)
							{
								mgame->currentExit = 0;
							}
							return 1;
						}
					if(mgame->startX+1 < mgame->mapWidth)
						if(destX == mgame->startX+1  && destY == mgame->startY)
						{
							mgame->currentExit++;
							if(mgame->currentExit*2 > mgame->dest.size()-1)
							{
								mgame->currentExit = 0;
							}
							return 1;
						}
					if(mgame->startY+1 < mgame->mapHeight )
						if(destY == mgame->startY+1 && destX == mgame->startX)
						{
							mgame->currentExit++;
							if(mgame->currentExit*2 > mgame->dest.size()-1)
							{
								mgame->currentExit = 0;
							}
							return 1;
						}
					if(mgame->startY != 0)
						if(destY == mgame->startY-1 && destX == mgame->startX)
						{
							mgame->currentExit++;
							if(mgame->currentExit*2 > mgame->dest.size()-1)
							{
								mgame->currentExit = 0;
							}
							return 1;
						}
					mgame->currentExit++;
					if(mgame->currentExit*2 > mgame->dest.size()-1)
					{
						mgame->currentExit = 0;
					}
					for(UINT i = 0; i < mgame->dest.size()-1; i = i + 2)
					{
						mgame->map[mgame->dest[i]][mgame->dest[i+1]] = 1;
					}
					stack *temp = FindPath(mgame->startX, mgame->startY, destX, destY, mgame->map, mgame->mapWidth, mgame->mapHeight);
					mgame->pathFinded = true;
					if(temp == NULL)
					{
						mgame->pathExist = false;
						for(UINT i = 0; i < mgame->mapWidth; i++)
							for(UINT j = 0; j < mgame->mapHeight; j++)
								if(mgame->map[i][j] != 1 && mgame->map[i][j] != 2 && mgame->map[i][j] != 3)
									mgame->map[i][j] = 0;
						mgame->map[mgame->startX][mgame->startY] = 3;
						for(UINT i = 0; i < mgame->dest.size()-1; i = i + 2)
							mgame->map[mgame->dest[i]][mgame->dest[i+1]] = 2;
					}
					else
					{
						for(UINT i = 0; i < mgame->mapWidth; i++)
							for(UINT j = 0; j < mgame->mapHeight; j++)
								if(mgame->map[i][j] != 1 && mgame->map[i][j] != 2 && mgame->map[i][j] != 3)
									mgame->map[i][j] = 0;
						mgame->pathExist = true;
						UINT x = mgame->startX;
						UINT y = mgame->startY;
						UINT oldx;
						UINT oldy;
						while(temp != NULL)
						{
							oldx = x;
							oldy = y;
							temp->Pop(temp,x,y);
							if(oldx > x)
								if(mgame->map[oldx][oldy] != 2 && mgame->map[oldx][oldy] != 3)
									mgame->map[oldx][oldy] = 7;
							if(oldx < x)
								if(mgame->map[oldx][oldy] != 2 && mgame->map[oldx][oldy] != 3)
									mgame->map[oldx][oldy] = 6;
							if(oldy > y)
								if(mgame->map[oldx][oldy] != 2 && mgame->map[oldx][oldy] != 3)
									mgame->map[oldx][oldy] = 5;
							if(oldy < y)
								if(mgame->map[oldx][oldy] != 2 && mgame->map[oldx][oldy] != 3)
									mgame->map[oldx][oldy] = 4;
						}
						if(x > destX)
							if(mgame->map[x][y] != 2 && mgame->map[x][y] != 3)
								mgame->map[x][y] = 7;
						if(x < destX)
							if(mgame->map[x][y] != 2 && mgame->map[x][y] != 3)
								mgame->map[x][y] = 6;
						if(y > destY)
							if(mgame->map[x][y] != 2 && mgame->map[x][y] != 3)
								mgame->map[x][y] = 5;
						if(y < destY)
							if(mgame->map[x][y] != 2 && mgame->map[x][y] != 3)
								mgame->map[x][y] = 4;
						mgame->map[mgame->startX][mgame->startY] = 3;
						for(UINT i = 0; i < mgame->dest.size()-1; i = i + 2)
							mgame->map[mgame->dest[i]][mgame->dest[i+1]] = 2;
					}
					mgame->showPath = true;
				}else
					mgame->showPath = false;
			}
			break;
	}
	return 1;
}

void MainGame::PerformCollisionDetection()
{
	bool needCount = false;
	//x-1
	if(engineHeaders.camera->eye.x < -14.5f)
	{
		if(this->startX != 0)
		{
			if(this->map[this->startX - 1][this->startY] != 1)
			{
				if(engineHeaders.camera->eye.x < -15.5f)
				{
					needCount= true;
					this->map[this->startX][this->startY] = 0;
					this->startX--;
					if(this->map[this->startX][this->startY] == 2)
					{
						this->mainGameState = 4;
						engineHeaders.camera->eye.x = 14.5f;
						this->pathFinded = false;
						return;
					}
					this->map[this->startX][this->startY] = 3;
					this->pathFinded = false;
					engineHeaders.camera->eye.x = 14.5f;
				}
			}else
			{
				engineHeaders.camera->eye.x = -14.5f;
			}
		}else
			engineHeaders.camera->eye.x = -14.5f;
	}
	//x+1
	if(engineHeaders.camera->eye.x > 14.5f)
	{
		if((this->startX + 1) < this->mapWidth)
		{
			if(this->map[this->startX + 1][this->startY] != 1)
			{
				if(engineHeaders.camera->eye.x > 15.5f)
				{
					needCount = true;
					this->map[this->startX][this->startY] = 0;
					this->startX++;
					if(this->map[this->startX][this->startY] == 2)
					{
						this->mainGameState = 4;
						engineHeaders.camera->eye.x = -14.5f;
						this->pathFinded = false;
						return;
					}
					this->map[this->startX][this->startY] = 3;
					this->pathFinded = false;
					engineHeaders.camera->eye.x = -14.5f;
				}
			}else
			{
				engineHeaders.camera->eye.x = 14.5f;
			}
		}else
			engineHeaders.camera->eye.x = 14.5f;
	}
	//y-1
	if(engineHeaders.camera->eye.z < -14.5f)
	{
		if(this->startY != 0)
		{
			if(this->map[this->startX][this->startY-1] != 1)
			{
				if(engineHeaders.camera->eye.z < -15.5f)
				{
					needCount = true;
					this->map[this->startX][this->startY] = 0;
					this->startY--;
					if(this->map[this->startX][this->startY] == 2)
					{
						this->mainGameState = 4;
						engineHeaders.camera->eye.z = 14.5f;
						this->pathFinded = false;
						return;
					}
					this->map[this->startX][this->startY] = 3;
					this->pathFinded = false;
					engineHeaders.camera->eye.z = 14.5f;
				}
			}else
			{
				engineHeaders.camera->eye.z = -14.5f;
			}
		}else
			engineHeaders.camera->eye.z = -14.5f;
	}
	//y+1
	if(engineHeaders.camera->eye.z > 14.5f)
	{
		if((this->startY + 1) < this->mapHeight)
		{
			if(this->map[this->startX][this->startY+1] != 1)
			{
				if(engineHeaders.camera->eye.z > 15.5f)
				{
					needCount = true;
					this->map[this->startX][this->startY] = 0;
					this->startY++;
					if(this->map[this->startX][this->startY] == 2)
					{
						this->mainGameState = 4;
						engineHeaders.camera->eye.z = -14.5f;
						this->pathFinded = false;
						return;
					}
					this->map[this->startX][this->startY] = 3;
					this->pathFinded = false;
					engineHeaders.camera->eye.z = -14.5f;
				}
			}else
			{
				engineHeaders.camera->eye.z = 14.5f;
			}
		}else
			engineHeaders.camera->eye.z = 14.5f;
	}
	if(needCount)
	{
		this->map2[this->startX][this->startY] = true;
		if(this->startX != 0)
		{
			this->map2[this->startX-1][this->startY] = true;
			if(this->startY != 0)
				this->map2[this->startX-1][this->startY-1] = true;
			if(this->startY +1 < this->mapHeight)
				this->map2[this->startX-1][this->startY+1] = true;
		}
		if(this->startX+1 < this->mapWidth)
		{
			this->map2[this->startX+1][this->startY] = true;
			if(this->startY != 0)
				this->map2[this->startX+1][this->startY-1] = true;
			if(this->startY+1 < this->mapHeight)
				this->map2[this->startX+1][this->startY+1] = true;
		}
		if(this->startY != 0)
			this->map2[this->startX][this->startY-1] = true;
		if(this->startY+1 < this->mapHeight)
			this->map2[this->startX][this->startY+1] = true;
	}
}