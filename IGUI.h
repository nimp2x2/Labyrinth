#ifndef IGUI_H
#define IGUI_H

#include "IInput.h"
#include "IFont.h"

class IGUIButton
{
	float				buttonX;
	float				buttonY;
	float				buttonWidth; //ширина и высота кнопки
	float				buttonHeight;
	//область, в которой регистрируются нажатия / hover кнопки
	float				offsetFromLeftTopX;
	float				offsetFromLeftTopY;
	float				offsetFromRightBottomX;
	float				offsetFromRightBottomY;

	bool				active; // работает ли кнопка
	bool				hoverSensible; // менять ли текстуры при наведении
	bool				pressSensible; // менять ли текстуры при нажатии
	bool				pressed; // нажата ли кнопка
	bool				hovered; // наxодится ли мышь над кнопкой
	DWORD				recoverTime; // время м/у нажатиями
	DWORD				lastTime;
	DWORD				recovering;

	ISprite				*sprite;
public:
	float				*normalStateTexCoord; // 0 - left top x, 1 - left top y, 2 - right bottom x, 3 - right bottom y
	float				*pressedStateTexCoord; // 0 - left top x, 1 - left top y, 2 - right bottom x, 3 - right bottom y
	float				*hoverStateTexCoord; // 0 - left top x, 1 - left top y, 2 - right bottom x, 3 - right bottom y

	void				SetNormalStateTexCoord(float x0, float y0, float x1, float y1);
	void				SetPressedStateTexCoord(float x0, float y0, float x1, float y1);
	void				SetHoverStateTexCoord(float x0, float y0, float x1, float y1);
	void				SetActiveAreaOffset(float offsetFromLeftTopX,float offsetFromLeftTopY,float offsetFromRightBottomX,float offsetFromRightBottomY);
	ISprite*			GetSprite(){ return this->sprite; }
	float				GetButtonX(){ return this->buttonX; }
	void				SetButtonX(float x){ this->buttonX = x; }
	float				GetButtonY(){ return this->buttonY; }
	void				SetButtonY(float y){ this->buttonY = y; }
	float				GetButtonWidth(){ return this->buttonWidth; }
	void				SetButtonWidth(float width){ this->buttonWidth = width; }
	float				GetButtonHeight(){ return this->buttonHeight; }
	void				SetButtonHeight(float height){ this->buttonHeight = height; }
	void				SetActive(bool active){ this->active = active; }
	void				SetHoverSens(bool hoverSens){ this->hoverSensible = hoverSens; }
	void				SetPressSens(bool pressSens){ this->pressSensible = pressSens; }
	void				SetRecoverTime(DWORD recoverTime){ this->recoverTime = recoverTime; }
	void				Update();

	IGUIButton(float x, float y,float width, float height, float z, IInput *input, LPDIRECT3DTEXTURE9 texture);
	~IGUIButton();
private:
	IInput				*input;
};

class IGUIEditBox
{
	float				boxX;
	float				boxY;
	float				boxWidth;
	float				boxHeight;

	bool				active; // активирован ли бокс?
	bool				focused; // сфокусирован ли ввод на боксе?
	wchar_t				*textBuffer;
	wchar_t				tempChar;
	int					rightBraceCharPos;
	int					leftBraceCharPos;
	DWORD				caretRecoverTime;
	float				*normalStateCoord;
	float				*focusedStateTexCoord;
	ISprite				*sprite;
	ISprite				*sprite2; // для каретки
	IFont				*font;
public:
	void				SetNormalTextureCoord(float x0, float y0, float x1, float y1);
	void				SetFocusedTexCoord(float x0, float y0, float x1, float y1);
	void				Update();
	float				GetEditX(){ return this->boxX; }
	void				SetEditX(float x){ this->boxX = x;}
	float				GetEditY(){ return this->boxY; }
	void				SetEditY(float y){ this->boxY = y;}
	float				GetEditWidth(){ return this->boxWidth; }
	void				SetEditWidth(float width){ this->boxWidth = width;}
	float				GetEditHeight(){ return this->boxHeight; }
	void				SetEditHeight(float height){ this->boxHeight = height;}
	void				SetActive(bool active){ this->active = active; }
	bool				GetFocusState(){ return this->focused; }
	ISprite*			GetSprite(int number = 0){ if(number == 0) return this->sprite; else return this->sprite2;}
	DWORD				GetCarretRecoverTime(){ return this->caretRecoverTime; }
	int					GetLeftBracePos(){ return this->leftBraceCharPos; }
	int					GetRightBracePos(){ return this->rightBraceCharPos; }
	wchar_t*			GetString(){ return this->textBuffer; }
	IFont*				GetFont(){ return this->font; }

	IGUIEditBox(float x, float y,float width, float height, float z, IInput *input,IFont *font,LPDIRECT3DTEXTURE9 texture);
	~IGUIEditBox();
private:
	IInput				*input;
};

#endif