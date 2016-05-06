#include "ISprite.h"

ISprite::ISprite(float spriteWidth,float spriteHeight,float z,DWORD spriteColor,LPDIRECT3DTEXTURE9 spriteTexture)
{
	this->width = spriteWidth;
	this->height = spriteHeight;
	this->color = spriteColor;
	this->texture = spriteTexture;
	this->z = z;
	this->hotX = this->width / 2;
	this->hotY = this->height / 2;
	this->SetTexturePosition(0.0f,0.0f,spriteWidth,spriteHeight);
}

ISprite::ISprite(float spriteWidth,float spriteHeight,float z,DWORD spriteColor)
{
	this->width = spriteWidth;
	this->height = spriteHeight;
	this->color = spriteColor;
	this->texture = NULL;
	this->z = z;
	this->hotX = this->width / 2;
	this->hotY = this->height / 2;
	this->texturePosition[0] = this->texturePosition[1] = this->texturePosition[2] = this->texturePosition[3] = 0.0f;
}

bool ISprite::SetTexturePosition(float leftTopCornerX,float leftTopCornerY,float rightBottomCornerX,float rightBottomCornerY)
{
	if(this->texture == NULL)
		return false;
	//получаем описание текстуры
	D3DSURFACE_DESC *pSurfaceDescription = new D3DSURFACE_DESC;
	this->texture->GetLevelDesc(0,pSurfaceDescription);
	
	if((rightBottomCornerX-leftTopCornerX > pSurfaceDescription->Width) || (rightBottomCornerY-leftTopCornerY > pSurfaceDescription->Height))
	{
		delete pSurfaceDescription;
		return false;
	}

	this->texturePosition[0] = leftTopCornerX / pSurfaceDescription->Width; //x0
	this->texturePosition[1] = leftTopCornerY / pSurfaceDescription->Height; //y0
	this->texturePosition[2] = rightBottomCornerX / pSurfaceDescription->Width; //x1
	this->texturePosition[3] = rightBottomCornerY / pSurfaceDescription->Height; //y1

	delete pSurfaceDescription;

	return true;
}

//*********************************************************************//
//**устанавливает центр спрайта относительно его верхнего левого угла**//
//*********************************************************************//
void ISprite::SetHotSpot(float X, float Y)
{
	this->hotX = X;
	this->hotY = Y;
}

void ISprite::GetTexturePos(float &x0, float &y0, float &x1, float &y1)
{
	x0 = this->texturePosition[0];
	y0 = this->texturePosition[1];
	x1 = this->texturePosition[2];
	y1 = this->texturePosition[3];
}