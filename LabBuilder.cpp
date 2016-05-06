#include "LabBuilder.h"

Builder::Builder()
{
	this->builderState = 0;
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
	this->mapWidth = 10;
	this->mapHeight = 10;
	this->map = NULL;
	this->filename = NULL;
	this->filenamepos = 0;
	this->widthChosen = false;
	this->chooseString = new wchar_t[256];
	this->chooseString[0] = 0;
	this->stringPos = 0;
}

Builder::~Builder()
{
	if(this->emptyBlock != NULL)
		delete this->emptyBlock;
	if(this->wall != NULL)
		delete this->wall;
	if(this->finishPoint != NULL)
		delete this->finishPoint;
	if(this->startPoint != NULL)
		delete this->startPoint;
	if(this->map != NULL)
	{
		for(UINT i = 0; i < this->mapWidth; i++)
			delete []map[i];
		delete []map;
	}
	if(this->filename != NULL)
		delete []this->filename;
	if(this->chooseString != NULL)
		delete this->chooseString;
}

bool Builder::Render(Builder *builder)
{
	switch(builder->builderState)
	{
	case 1:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 + (engineHeaders.font->GetScale()*32.0f), C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Ширина");
		if(builder->widthChosen)
			engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"%u", builder->mapWidth);
		else
			engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"%s", builder->chooseString);
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 - (engineHeaders.font->GetScale()*32.0f), C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Высота");
		if(!builder->widthChosen)
			engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 - (engineHeaders.font->GetScale()*32.0f*2), C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"%u", builder->mapHeight);
		else
			engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 - (engineHeaders.font->GetScale()*32.0f*2), C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"%s", builder->chooseString);
		engineHeaders.scene2d->sceneEnd();
		break;
	case 2:
		engineHeaders.scene2d->sceneBegin();
		//debug info 
		//engineHeaders.scene2d->DrawSimpleText(engineHeaders.font, 2.0f, 600.0f,  0, 256, L"offsetx = %d, offsety = %d\n currentX = %d, currentY = %d",builder->offsetx,builder->offsety,builder->currentPosX, builder->currentPosY);
		if(builder->mapWidth > engineHeaders.maxXBlocks || builder->mapHeight > engineHeaders.maxYBlocks)
		{
			builder->icounter = 0;
			builder->jcounter = 0;
			builder->ymax = engineHeaders.maxYBlocks < builder->mapHeight ? engineHeaders.maxYBlocks : builder->mapHeight;
			builder->xmax = engineHeaders.maxXBlocks < builder->mapWidth ? engineHeaders.maxXBlocks : builder->mapWidth;
			builder->xadd = 0;
			//xmax and iset setting
			if(builder->offsetx > 0 && builder->currentPosX != builder->mapWidth - engineHeaders.maxXBlocks)
			{
				builder->xmax = engineHeaders.maxXBlocks < builder->mapWidth ? engineHeaders.maxXBlocks+1 : builder->mapWidth;
			}else if(builder->offsetx < 0  && builder->currentPosX != 0)
			{
				builder->xmax = engineHeaders.maxXBlocks < builder->mapWidth ? engineHeaders.maxXBlocks : builder->mapWidth;
				builder->xadd = -1;
				builder->icounter = -1;
			}
			//ymax setting
			if(builder->offsety < 0 && builder->currentPosY != 0)
			{
				builder->ymax = engineHeaders.maxYBlocks < builder->mapHeight ? engineHeaders.maxYBlocks : builder->mapHeight;
				builder->jcounter = -1;
			}else if(builder->offsety > 0 && builder->currentPosY != builder->mapHeight - engineHeaders.maxYBlocks)
			{
				builder->ymax = engineHeaders.maxYBlocks < builder->mapHeight ? engineHeaders.maxYBlocks+1 : builder->mapHeight;
			}
			//builder->xadd = builder->icounter = 0;
			//main drawing cycle
			while(builder->jcounter < builder->ymax)
			{
				while(builder->icounter < builder->xmax)
				{
					if(builder->map[builder->currentPosX+builder->icounter][builder->currentPosY+builder->jcounter] == 0)
						engineHeaders.scene2d->DrawSprite(builder->emptyBlock, (float)(builder->icounter*16-builder->offsetx), (float)(builder->jcounter*16-builder->offsety+16));
					else if(builder->map[builder->currentPosX+builder->icounter][builder->currentPosY+builder->jcounter] == 1)
						engineHeaders.scene2d->DrawSprite(builder->wall, (float)(builder->icounter*16-builder->offsetx), (float)(builder->jcounter*16-builder->offsety+16));
					else if(builder->map[builder->currentPosX+builder->icounter][builder->currentPosY+builder->jcounter] == 2)
						engineHeaders.scene2d->DrawSprite(builder->finishPoint, (float)(builder->icounter*16-builder->offsetx), (float)(builder->jcounter*16-builder->offsety+16));
					else engineHeaders.scene2d->DrawSprite(builder->startPoint, (float)(builder->icounter*16-builder->offsetx), (float)(builder->jcounter*16-builder->offsety+16));
					builder->icounter+=1;
				}
				builder->icounter = builder->xadd;
				builder->jcounter += 1;
			}
		}
		else{
			for(UINT jcounter = 0; jcounter < builder->mapHeight; jcounter++)
				for(UINT icounter = 0; icounter < builder->mapWidth; icounter++)
				{
					if(builder->map[icounter][jcounter] == 0)
						engineHeaders.scene2d->DrawSprite(builder->emptyBlock,(float)(icounter*16),(float)(jcounter*16)+16+2);
					else if(builder->map[icounter][jcounter] == 1)
						engineHeaders.scene2d->DrawSprite(builder->wall,(float)(icounter*16),(float)(jcounter*16)+16+2);
					else if(builder->map[icounter][jcounter] == 2)
						engineHeaders.scene2d->DrawSprite(builder->finishPoint,(float)(icounter*16),(float)(jcounter*16)+16+2);
					else if(builder->map[icounter][jcounter] == 3)
						engineHeaders.scene2d->DrawSprite(builder->startPoint,(float)(icounter*16),(float)(jcounter*16)+16+2);
				}
		}
		engineHeaders.scene2d->sceneEnd();
		break;
	case 3:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 + (engineHeaders.font->GetScale()*32.0f), C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Открыть существующий файл или создать новый?");
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Создать\nОткрыть");
		engineHeaders.scene2d->sceneEnd();
		break;
	case 4:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 + (engineHeaders.font->GetScale()*32.0f), C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Введите имя файла(backspace стереть)");
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"%s", builder->filename);
		engineHeaders.scene2d->sceneEnd();
		break;
	case 5:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 + (engineHeaders.font->GetScale()*32.0f), C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Не удалось открыть файл");
		engineHeaders.scene2d->sceneEnd();
		break;
	case 6:
		engineHeaders.scene2d->sceneBegin();
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2 + (engineHeaders.font->GetScale()*32.0f), C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"Введите имя файла(backspace стереть)");
		engineHeaders.scene2d->DrawTextExW(engineHeaders.font, engineHeaders.currentWidth/2, engineHeaders.currentHeight/2, C2DFONT_TEXT_ALIGN_CENTER, 0, 256, L"%s", builder->filename);
		engineHeaders.scene2d->sceneEnd();
	}
	return true;
}

DWORD Builder::Logic(Builder *builder)
{
	if(engineHeaders.input->GetKeyPressNumber(VK_ESCAPE) > 0)
	{
		builder->builderState = 0;
		return 3;
	}
	wchar_t buf = 0;
	switch(builder->builderState)
	{
	case 0:
		if(builder->map != NULL)
		{
			for(UINT i = 0; i < builder->mapWidth; i++)
				delete []builder->map[i];
			delete []builder->map;
		}
		if(builder->filename != NULL)
			delete []builder->filename;
		builder->filenamepos = 0;
		builder->filename = NULL;
		builder->mapWidth = 10;
		builder->mapHeight = 10;
		builder->map = NULL;
		builder->filename = NULL;
		builder->builderState = 0;
		builder->currentPosX = 0;
		builder->currentPosY = 0;
		builder->offsetx = 0;
		builder->offsety = 0;
		builder->xmax = 0;
		builder->ymax = 0;
		builder->xadd = 0;
		builder->yadd = 0;
		builder->icounter = 0;
		builder->jcounter = 0;
		builder->dx = 0;
		builder->dy = 0;
		builder->oldx = 0;
		builder->oldy = 0;
		builder->drag = false;
		builder->startX = 0;
		builder->startY = 0;
		builder->builderState = 3;
		builder->dest.clear();
		break;
	case 1:
		if(engineHeaders.input->GetKeyPressNumber(VK_BACK) > 0)
		{
			if(builder->stringPos > 0)
			{
				builder->chooseString[builder->stringPos] = 0;
				builder->chooseString[builder->stringPos-1] = 0;
				builder->stringPos--;
			}
		}
		buf = engineHeaders.input->GetUnicodeFromKeyQueue();
		if(buf != NULL && buf >= 0x30 && buf <= 0x39 )
		{
			if(builder->stringPos < 255)
			{
				builder->chooseString[builder->stringPos] = buf;
				builder->chooseString[builder->stringPos+1] = 0;
				builder->stringPos++;
			}
		}
		if(!builder->widthChosen)
		{
			if(engineHeaders.input->GetKeyPressNumber(VK_RETURN) > 0)
			{
				builder->widthChosen = true;
				builder->mapWidth = _wtoi(builder->chooseString);
				builder->chooseString[0] = 0;
				builder->stringPos = 0;
				if(builder->mapWidth < 10)
					builder->mapWidth = 10;
			}
		}else
		{
			if(engineHeaders.input->GetKeyPressNumber(VK_RETURN) > 0)
			{
				builder->widthChosen = false;
				builder->mapHeight = _wtoi(builder->chooseString);
				builder->chooseString[0] = 0;
				builder->stringPos = 0;
				if(builder->mapHeight < 10)
					builder->mapHeight = 10;
				builder->currentPosX = builder->currentPosY = 0;
				builder->offsetx = builder->offsety = 0;
				builder->builderState = 2;
				if(builder->map != NULL)
				{
					for(UINT i = 0; i < builder->mapWidth; i++)
						delete []builder->map[i];
					delete []builder->map;
					builder->map = NULL;
				}
				builder->map = new UINT*[builder->mapWidth];
				for(UINT i = 0; i < builder->mapWidth; i++)
				{
					builder->map[i] = new UINT[builder->mapHeight];
					for(UINT j = 0; j < builder->mapHeight; j++)
						builder->map[i][j] = 0;
				}
			}
		}
		break;
	case 2:
			if(engineHeaders.input->GetCurrentKeyState(VK_ESCAPE))
			{
				builder->builderState = 0;
				if(builder->map != NULL)
				{
					for(UINT i = 0; i < builder->mapWidth; i++)
						delete []builder->map[i];
					delete []builder->map;
					builder->map = NULL;
				}
				builder->mapWidth = builder->mapHeight = 10;
				builder->builderState = 0;
				return 3;
			}
			if(engineHeaders.input->GetCurrentKeyState(0x53))
			{
				builder->builderState = 6;
				if(builder->filename != NULL)
					delete []builder->filename;
				builder->filenamepos = 0;
				builder->filename = new wchar_t[256];
				builder->filename[0] = 0;
				return 1;
			}
			if(engineHeaders.input->GetKeyPressNumber(VK_LBUTTON) > 0)
			{
				builder->xmax = engineHeaders.input->GetMouseX()+builder->offsetx;
				builder->ymax = engineHeaders.input->GetMouseY()+builder->offsety;
				builder->icounter = 0;
				builder->jcounter = 0;
				while(builder->xmax >= 16)
				{
					builder->xmax -= 16;
					builder->icounter++;
				}
				while(builder->ymax >= 16)
				{
					builder->ymax -= 16;
					builder->jcounter++;
				}
				builder->icounter += builder->currentPosX;
				builder->jcounter += builder->currentPosY;
				if((UINT)builder->icounter < builder->mapWidth && (UINT)builder->jcounter < builder->mapHeight)
				{
					if(builder->map[builder->icounter][builder->jcounter] == 0)
						builder->map[builder->icounter][builder->jcounter] = 1;
					else if(builder->map[builder->icounter][builder->jcounter] == 1)
						builder->map[builder->icounter][builder->jcounter] = 0;
				}
			}
			//start pos
			if(engineHeaders.input->GetKeyPressNumber(0x31) > 0)
			{
				builder->xmax = engineHeaders.input->GetMouseX()+builder->offsetx;
				builder->ymax = engineHeaders.input->GetMouseY()+builder->offsety;
				builder->icounter = 0;
				builder->jcounter = 0;
				while(builder->xmax >= 16)
				{
					builder->xmax -= 16;
					builder->icounter++;
				}
				while(builder->ymax >= 16)
				{
					builder->ymax -= 16;
					builder->jcounter++;
				}
				builder->icounter += builder->currentPosX;
				builder->jcounter += builder->currentPosY;
				if((UINT)builder->icounter < builder->mapWidth && (UINT)builder->jcounter < builder->mapHeight)
				{
					if(builder->map[builder->icounter][builder->jcounter] == 0)
					{
						if(builder->map[builder->startX][builder->startY] == 3)
							builder->map[builder->startX][builder->startY] = 0;
						builder->startX = builder->icounter;
						builder->startY = builder->jcounter;
						builder->map[builder->startX][builder->startY] = 3;
					}
				}
			}

			//end pos
			if(engineHeaders.input->GetKeyPressNumber(0x32) > 0)
			{
				builder->xmax = engineHeaders.input->GetMouseX()+builder->offsetx;
				builder->ymax = engineHeaders.input->GetMouseY()+builder->offsety;
				builder->icounter = 0;
				builder->jcounter = 0;
				while(builder->xmax >= 16)
				{
					builder->xmax -= 16;
					builder->icounter++;
				}
				while(builder->ymax >= 16)
				{
					builder->ymax -= 16;
					builder->jcounter++;
				}
				builder->icounter += builder->currentPosX;
				builder->jcounter += builder->currentPosY;
				if((UINT)builder->icounter < builder->mapWidth && (UINT)builder->jcounter < builder->mapHeight)
				{
					bool founded = false;
					if(!builder->dest.empty())
					{
						for(UINT i = 0; i < (UINT)(builder->dest.size()-1); i = i+2)
						{
							if(builder->dest[i] == builder->icounter && builder->dest[i+1] == builder->jcounter)
							{
								builder->map[builder->icounter][builder->jcounter] = 0;
								builder->dest.erase(builder->dest.begin()+i);
								builder->dest.erase(builder->dest.begin()+i);
								founded = true;
								break;
							}
						}
					}

					if(builder->map[builder->icounter][builder->jcounter] == 0 && !founded)
					{
						builder->dest.push_back(builder->icounter);
						builder->dest.push_back(builder->jcounter);
						builder->map[builder->icounter][builder->jcounter] = 2;
					}
				}
			}
			if(builder->mapWidth > engineHeaders.maxXBlocks || builder->mapHeight > engineHeaders.maxYBlocks)
			{
				if(engineHeaders.input->GetKeyPressNumber(VK_RBUTTON))
				{
					builder->oldx = engineHeaders.input->GetMouseX();
					builder->oldy = engineHeaders.input->GetMouseY();
					builder->dx = 0;
					builder->dy = 0;
					builder->drag = true;
				}
				if(engineHeaders.input->GetCurrentKeyState(VK_RBUTTON) && builder->drag)
				{
					//x
					if(builder->mapWidth > (engineHeaders.maxXBlocks))
					{
						builder->dx = -engineHeaders.input->GetMouseX() + builder->oldx;
						builder->offsetx += builder->dx;
					}
					else
					{
						builder->dx = 0;
					}
					
					if(builder->offsetx > 0 && builder->currentPosX == (builder->mapWidth-(engineHeaders.maxXBlocks)))
						builder->offsetx = 0;
					if(builder->offsetx < 0 && builder->currentPosX == 0)
						builder->offsetx = 0;
					if(builder->offsetx >= 16)
					{
						while(builder->offsetx >= 16)
						{
							builder->offsetx -= 16;
								if(builder->currentPosX < (builder->mapWidth-(engineHeaders.maxXBlocks)))
									builder->currentPosX++;
							else
								builder->offsetx = 0;
					
						}
					}
					if(builder->offsetx <= -16)
					{
						while(builder->offsetx <= -16)
						{
							builder->offsetx += 16;
							if(builder->currentPosX > 0)
								builder->currentPosX--;
							else
								builder->offsetx = 0;
						}
					}
					builder->oldx = engineHeaders.input->GetMouseX();
					//y
					if(builder->mapHeight > (engineHeaders.maxYBlocks))
					{
						builder->dy = engineHeaders.input->GetMouseY() - builder->oldy;
						builder->offsety -= builder->dy;
					}
					else
						builder->dy = 0;

					if(builder->offsety > 0 && builder->currentPosY == (builder->mapHeight-(engineHeaders.maxYBlocks)))
						builder->offsety = 0;
					if(builder->offsety < 0 && builder->currentPosY == 0)
						builder->offsety = 0;
					if(builder->offsety >= 16)
					{
						while(builder->offsety >= 16)
						{
							builder->offsety -= 16;
							if(builder->currentPosY < (builder->mapHeight-(engineHeaders.maxYBlocks)))
								builder->currentPosY++;
							else
								builder->offsety = 0;
					
						}
					}
					if(builder->offsety <= -16)
					{
						while(builder->offsety <= -16)
						{
							builder->offsety += 16;
							if(builder->currentPosY > 0)
								builder->currentPosY--;
							else
								builder->offsety = 0;
						}
					}
					builder->oldy = engineHeaders.input->GetMouseY();
				}else
				{
					builder->drag = false;
					builder->dx = 0;
					builder->dy = 0;
				}
			}
		break;
	case 3:
		//back button
		if(engineHeaders.input->GetKeyPressNumber(VK_LBUTTON))
		{
			if(engineHeaders.input->GetMouseX() > engineHeaders.currentWidth/2 - 80 && engineHeaders.input->GetMouseX() < engineHeaders.currentWidth/2 + 80
				&& engineHeaders.input->GetMouseY() > (engineHeaders.currentHeight/2 - (engineHeaders.font->GetScale()*32.0f)) && engineHeaders.input->GetMouseY() < engineHeaders.currentHeight/2)
				builder->builderState = 1;
			if(engineHeaders.input->GetMouseX() > engineHeaders.currentWidth/2 - 80 && engineHeaders.input->GetMouseX() < engineHeaders.currentWidth/2 + 80
				&& engineHeaders.input->GetMouseY() > (engineHeaders.currentHeight/2 - (engineHeaders.font->GetScale()*32.0f)*2) && engineHeaders.input->GetMouseY() < engineHeaders.currentHeight/2- (engineHeaders.font->GetScale()*32.0f))
			{
				builder->builderState = 4;	
				builder->filename = new wchar_t[256];
				builder->filename[0] = 0;
				builder->filenamepos = 0;
			}
		}
		break;
	case 4:
		if(engineHeaders.input->GetKeyPressNumber(VK_BACK) > 0)
		{
			if(builder->filenamepos > 0)
			{
				builder->filename[builder->filenamepos] = 0;
				builder->filename[builder->filenamepos-1] = 0;
				builder->filenamepos--;
			}
		}
		buf = engineHeaders.input->GetUnicodeFromKeyQueue();
		if(buf != NULL && ( (buf >= 0x41 && buf <= 0x5A) || (buf >= 0x61 && buf <= 0x7A) || (buf == 0x2E) || (buf >= 0x30 && buf <= 0x39) ) )
		{
			if(builder->filenamepos < 255)
			{
				builder->filename[builder->filenamepos] = buf;
				builder->filename[builder->filenamepos+1] = 0;
				builder->filenamepos++;
			}
		}
		if(engineHeaders.input->GetKeyPressNumber(VK_RETURN) > 0)
		{
			if(GetFileAttributes(builder->filename) == DWORD(-1))
			{
				builder->builderState = 5;
				return 1;
			}
			builder->builderState = 2;
			FILE *file;
			_wfopen_s(&file,L"data.bin",L"rb");
			fread(&builder->mapWidth, sizeof(UINT), 1, file);
			fread(&builder->mapHeight, sizeof(UINT), 1, file);
			fread(&builder->startX, sizeof(UINT), 1, file);
			fread(&builder->startY, sizeof(UINT), 1, file);
			UINT size;
			fread(&size ,sizeof(UINT), 1, file);
			builder->dest.clear();
			builder->dest.resize(size);
			for(UINT i = 0; i < size; i++)
				fread(&builder->dest[i], sizeof(UINT), 1, file);
			builder->map = new UINT*[builder->mapWidth];
			for(UINT i = 0; i < builder->mapWidth; i++)
			{
				builder->map[i] = new UINT[builder->mapHeight];
				for(UINT j = 0; j < builder->mapHeight; j++)
					builder->map[i][j] = 0;
			}
			for(UINT i = 0; i < builder->mapWidth; i++)
				for(UINT j = 0; j < builder->mapHeight; j++)
					fread(&builder->map[i][j], sizeof(UINT), 1, file);
			fclose(file);
		}
		break;
	case 6:
		if(engineHeaders.input->GetKeyPressNumber(VK_BACK) > 0)
		{
			if(builder->filenamepos > 0)
			{
				builder->filename[builder->filenamepos] = 0;
				builder->filename[builder->filenamepos-1] = 0;
				builder->filenamepos--;
			}
		}
		buf = engineHeaders.input->GetUnicodeFromKeyQueue();
		if(buf != NULL && ( (buf >= 0x41 && buf <= 0x5A) || (buf >= 0x61 && buf <= 0x7A) || (buf == 0x2E) || (buf >= 0x30 && buf <= 0x39) ) )
		{
			if(builder->filenamepos < 255)
			{
				builder->filename[builder->filenamepos] = buf;
				builder->filename[builder->filenamepos+1] = 0;
				builder->filenamepos++;
			}
		}
		if(engineHeaders.input->GetKeyPressNumber(VK_RETURN) > 0)
		{
			FILE *file;
			builder->builderState = 0;
			if(builder->filenamepos == 0)
				_wfopen_s(&file,L"data.bin",L"wb");
			else
				_wfopen_s(&file,builder->filename,L"wb");
			if(file != NULL)
			{
				if(builder->map[builder->startX][builder->startY] != 3)
				{
					bool founded = false;
					for(UINT i = 0; i < builder->mapWidth; i++)
					{
						if(founded)
							break;
						for(UINT j = 0; j < builder->mapHeight; j++)
							if(builder->map[i][j] == 0 || builder->map[i][j] == 1)
							{
								builder->map[i][j] = 3;
								builder->startX = i;
								builder->startY = j;
								founded = true;
								break;
							}
					}
					if(!founded)
					{
						builder->startX = builder->dest[builder->dest.size()-2];
						builder->startY = builder->dest[builder->dest.size()-1];
						builder->dest.pop_back();
						builder->dest.pop_back();
					}
				}
				if(builder->dest.empty())
				{
					bool founded = false;
					for(UINT i = 0; i < builder->mapWidth; i++)
					{
						if(founded)
							break;
						for(UINT j = 0; j < builder->mapHeight; j++)
							if(builder->map[i][j] == 0 || builder->map[i][j] == 1)
							{
								builder->dest.push_back(i);
								builder->dest.push_back(j);
								builder->map[i][j] = 2;
								founded = true;
								break;
							}
					}
				}
				fwrite(&builder->mapWidth, sizeof(UINT), 1, file);
				fwrite(&builder->mapHeight, sizeof(UINT), 1, file);
				fwrite(&builder->startX, sizeof(UINT), 1, file);
				fwrite(&builder->startY, sizeof(UINT), 1, file);
				UINT size = (UINT)builder->dest.size();
				fwrite(&size ,sizeof(UINT), 1, file);
				for(UINT i = 0; i < size; i++)
					fwrite(&builder->dest[i], sizeof(UINT), 1, file);
				for(UINT i = 0; i < builder->mapWidth; i++)
					for(UINT j = 0; j < builder->mapHeight; j++)
						fwrite(&builder->map[i][j],sizeof(UINT),1,file);
				fclose(file);
			}
			if(builder->map != NULL)
			{
				for(UINT i = 0; i < builder->mapWidth; i++)
					delete []builder->map[i];
				delete []builder->map;
				builder->map = NULL;
			}
			builder->mapWidth = builder->mapHeight = 10;
			return 3;
		}
		break;
	}
	return 1;
}