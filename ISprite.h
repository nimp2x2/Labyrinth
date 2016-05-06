#ifndef ISPRITE_H
#define ISPRITE_H

#include "ILogger.h"

class ISprite
{
	float								width;//������
	float								height;//������
	DWORD								color;//����
	LPDIRECT3DTEXTURE9					texture; //��������� �� ��������
	float								texturePosition[4]; //���������� ���������� (0 - x0, 1 - y0, 2 - x1, 3 - y1)
	float								z;// �������
	float								hotX;//����� ������� �� ��� �
	float								hotY;//����� ������� �� ��� �	
public:
	bool								SetTexturePosition(float leftTopCornerX,float leftTopCornerY,float rightBottomCornerX,float rightBottomCornerY); 
	void								GetTexturePos(float &x0, float &y0, float &x1, float &y1);
	void								SetHotSpot(float X, float Y);//��������� ������ ������� ������������ ��� ������ �������� ����
	void								GetHotSpot(float &x, float &y){ x = this->hotX; y = this->hotY; }
	void								SetZ(float z){ this->z = z; }
	float								GetZ(){ return this->z; }
	void								SetColor(DWORD color){ this->color = color; }
	DWORD								GetColor() { return this->color; }
	void								SetTexture(LPDIRECT3DTEXTURE9 texture){ this->texture = texture; }
	LPDIRECT3DTEXTURE9					GetTexture(){ return this->texture; }
	void								SetWidth(float width){ this->width = width; }
	float								GetWidth(){ return this->width; }
	void								SetHeight(float height){ this->height = height; }
	float								GetHeight(){ return this->height; }

	ISprite(float spriteWidth,float spriteHeight,float z,DWORD spriteColor,LPDIRECT3DTEXTURE9 spriteTexture); 
	ISprite(float spriteWidth,float spriteHeight,float z,DWORD spriteColor);
}; 

#endif