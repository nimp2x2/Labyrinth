#ifndef IANIM_H
#define IANIM_H

#include "ISprite.h"

//***************************************************//
//**������������ ���� ������ ��� ������ � ���������**//
//***************************************************//
class IAnim
{
	ISprite					*sprite; //��������� �� ������
	float					textureStartXPos; //� ���������� ������� �����
	float					textureStartYPos; //� ���������� ������� �����
	float					texFrameWidth;//������ ������ �����
	float					texFrameHeight;//������ ������ �����

	unsigned int			rateCounter; //������� ����� ������
	unsigned int			currentFrame; //������� ����
	unsigned int			frames; // ���������� ������ � ��������, �� 1 �� MAX_UNSIGNED_INT
	unsigned int			frameRate; //�������� ����� ������ (����� ����������, ������� ��� ������ ���� ������ ����� Update(), ����� �������� ����)
	bool					play; //�������������� �� ��������?
	bool					twoSidePlay; // ����� ���������� ����� �������� �������� �� 0 � ����� �� ���������� �����
	bool					twoSideFlag;

public:
	bool					Update(); //����� ��� ���������� ������
	void					SetStartTexturePositionAndSize(float x,float y,float width,float height); //���������� ������� �����
	void					SetCurrentFrame(unsigned int frame); //������������� ������������ ���� � ������������������
	ISprite*				GetSprite(){ return this->sprite; }
	void					SetTwoSidePlay(bool twoSide) { this->twoSidePlay = twoSide; }
	bool					GetTwoSidePlay() {return this->twoSidePlay; }

	IAnim(float frameWidth,float frameHeight,float z,DWORD animColor,unsigned int frames,unsigned int frameRate,LPDIRECT3DTEXTURE9 texture);
	~IAnim(); 
};

#endif