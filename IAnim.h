#ifndef IANIM_H
#define IANIM_H

#include "ISprite.h"

//***************************************************//
//**заголовочный файл класса для работы с анимацией**//
//***************************************************//
class IAnim
{
	ISprite					*sprite; //указатель на спрайт
	float					textureStartXPos; //х координата первого кадра
	float					textureStartYPos; //у координата первого кадра
	float					texFrameWidth;//ширина одного кадра
	float					texFrameHeight;//высота одного кадра

	unsigned int			rateCounter; //счетчик смены кадров
	unsigned int			currentFrame; //текущий кадр
	unsigned int			frames; // количество кадров в анимации, от 1 до MAX_UNSIGNED_INT
	unsigned int			frameRate; //скорость смены кадров (число показывает, сколько раз должен быть вызван метод Update(), чтобы сменился кадр)
	bool					play; //воспроизводить ли анимацию?
	bool					twoSidePlay; // после последнего кадра начинать убавлять до 0 и снова до последнего кадра
	bool					twoSideFlag;

public:
	bool					Update(); //метод для обновления кадров
	void					SetStartTexturePositionAndSize(float x,float y,float width,float height); //координаты первого кадра
	void					SetCurrentFrame(unsigned int frame); //устанавливает определенный кадр в последовательности
	ISprite*				GetSprite(){ return this->sprite; }
	void					SetTwoSidePlay(bool twoSide) { this->twoSidePlay = twoSide; }
	bool					GetTwoSidePlay() {return this->twoSidePlay; }

	IAnim(float frameWidth,float frameHeight,float z,DWORD animColor,unsigned int frames,unsigned int frameRate,LPDIRECT3DTEXTURE9 texture);
	~IAnim(); 
};

#endif