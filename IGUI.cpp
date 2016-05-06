#include "IGUI.h"

IGUIButton::IGUIButton(float x, float y,float width, float height, float z, IInput *input,LPDIRECT3DTEXTURE9 texture)
{
	this->buttonX = x;
	this->buttonY = y;
	this->input = input;
	this->active = true;
	this->hoverSensible = false;
	this->pressSensible = false;
	this->hovered = false;
	this->pressed = false;
	this->sprite = new ISprite(width,height,z,D3DCOLOR_XRGB(255,255,255));
	this->sprite->SetTexture(texture);
	this->buttonWidth = width;
	this->buttonHeight = height;
	this->offsetFromLeftTopX = 0.0f;
	this->offsetFromLeftTopY = 0.0f;
	this->offsetFromRightBottomX = 0.0f;
	this->offsetFromRightBottomY = 0.0f;
	this->normalStateTexCoord = new float[4];
	this->normalStateTexCoord[0] = this->normalStateTexCoord[1] = this->normalStateTexCoord[2] = this->normalStateTexCoord[3] = 0.0f;
	this->pressedStateTexCoord = NULL;
	this->hoverStateTexCoord = NULL;
	this->recoverTime = 0;
	this->recovering = false;
}

IGUIButton::~IGUIButton()
{
	SAFE_RELEASE(this->hoverStateTexCoord);
	SAFE_RELEASE(this->pressedStateTexCoord);
	SAFE_RELEASE(this->normalStateTexCoord);
	SAFE_RELEASE(this->sprite);
}

void IGUIButton::SetNormalStateTexCoord(float x0, float y0, float x1, float y1)
{
	if(this->sprite->GetTexture() == NULL)
		return;
	//получаем описание текстуры
	D3DSURFACE_DESC *surfaceDescription = new D3DSURFACE_DESC;
	this->sprite->GetTexture()->GetLevelDesc(0,surfaceDescription);
	
	if((x1-x0 > surfaceDescription->Width) || (y1-y0 > surfaceDescription->Height))
		return;

	this->normalStateTexCoord[0] = x0 / surfaceDescription->Width; //x0
	this->normalStateTexCoord[1] = y0 / surfaceDescription->Height; //y0

	this->normalStateTexCoord[2] = x1 / surfaceDescription->Width; //x1
	this->normalStateTexCoord[3] = y1 / surfaceDescription->Height; //y1

	delete surfaceDescription;
}

void IGUIButton::SetPressedStateTexCoord(float x0, float y0, float x1, float y1)
{
	if(this->sprite->GetTexture() == NULL)
		return;

	//получаем описание текстуры
	D3DSURFACE_DESC *surfaceDescription = new D3DSURFACE_DESC;
	this->sprite->GetTexture()->GetLevelDesc(0,surfaceDescription);
	
	if((x1-x0 > surfaceDescription->Width) || (y1-y0 > surfaceDescription->Height))
		return;

	this->pressedStateTexCoord = new float[4];
	this->pressedStateTexCoord[0] = x0 / surfaceDescription->Width; //x0
	this->pressedStateTexCoord[1] = y0 / surfaceDescription->Height; //y0

	this->pressedStateTexCoord[2] = x1 / surfaceDescription->Width; //x1
	this->pressedStateTexCoord[3] = y1 / surfaceDescription->Height; //y1

	delete surfaceDescription;
	this->pressSensible = true;
}

void IGUIButton::SetHoverStateTexCoord(float x0, float y0, float x1, float y1)
{
	if(this->sprite->GetTexture() == NULL)
		return;

	//получаем описание текстуры
	D3DSURFACE_DESC *surfaceDescription = new D3DSURFACE_DESC;
	this->sprite->GetTexture()->GetLevelDesc(0,surfaceDescription);
	
	if((x1-x0 > surfaceDescription->Width) || (y1-y0 > surfaceDescription->Height))
		return;

	this->hoverStateTexCoord = new float[4];
	this->hoverStateTexCoord[0] = x0 / surfaceDescription->Width; //x0
	this->hoverStateTexCoord[1] = y0 / surfaceDescription->Height; //y0

	this->hoverStateTexCoord[2] = x1 / surfaceDescription->Width; //x1
	this->hoverStateTexCoord[3] = y1 / surfaceDescription->Height; //y1

	delete surfaceDescription;
	this->hoverSensible = true;
}

void IGUIButton::Update()
{
	if(this->recovering)
	{
		if(timeGetTime() - this->lastTime >= this->recoverTime)
			this->recovering = false;
	}
	if(this->pressed)
		if(this->input->GetCurrentKeyState(VK_LBUTTON))
			return;
		else
		{
			this->pressed = false;
			this->recovering = true;
			this->lastTime = timeGetTime();
			this->sprite->SetTexturePosition(this->normalStateTexCoord[0],this->normalStateTexCoord[1],this->normalStateTexCoord[2],this->normalStateTexCoord[3]);
			return;
		}
	if(this->active)
		if((this->input->GetMouseX() >= (this->buttonX+this->offsetFromLeftTopX)) && 
			(this->input->GetMouseY() <= (this->buttonY-this->offsetFromLeftTopY)) && 
			(this->input->GetMouseX() <= (this->buttonWidth+this->buttonX-this->offsetFromRightBottomX)) && 
			(this->input->GetMouseY() >= (-this->buttonHeight+this->buttonY+this->offsetFromRightBottomY)))
		{
			if(this->input->GetCurrentKeyState(VK_LBUTTON))
			{
				if(!this->pressed && !this->recovering)
				{
					this->pressed = true;
					this->hovered = false;
					if(this->pressSensible)
					{
						this->sprite->SetTexturePosition(this->pressedStateTexCoord[0],this->pressedStateTexCoord[1],this->pressedStateTexCoord[2],this->pressedStateTexCoord[3]);
					}
				}
			}else{
				if(this->hoverSensible && !this->hovered)
				{
					this->hovered =  true;
					this->sprite->SetTexturePosition(this->hoverStateTexCoord[0],this->hoverStateTexCoord[1],this->hoverStateTexCoord[2],this->hoverStateTexCoord[3]);
				}
			}
		}else{
			if(this->hovered)
			{
				this->hovered = false;
				this->sprite->SetTexturePosition(this->normalStateTexCoord[0],this->normalStateTexCoord[1],this->normalStateTexCoord[2],this->normalStateTexCoord[3]);
			}
		}
}

void IGUIButton::SetActiveAreaOffset(float offsetFromLeftTopX,float offsetFromLeftTopY,float offsetFromRightBottomX,float offsetFromRightBottomY)
{
	this->offsetFromLeftTopX = offsetFromLeftTopX;
	this->offsetFromLeftTopY = offsetFromLeftTopY;
	this->offsetFromRightBottomX = offsetFromRightBottomX;
	this->offsetFromRightBottomY = offsetFromRightBottomY;
}

IGUIEditBox::IGUIEditBox(float x, float y,float width, float height, float z, IInput *input,IFont *font,LPDIRECT3DTEXTURE9 texture)
{
	this->boxX = x;
	this->boxY = y;
	this->boxWidth = width;
	this->boxHeight = height;
	this->sprite = new ISprite(this->boxWidth,this->boxHeight,z,D3DCOLOR_XRGB(255,255,255));
	this->sprite2 = new ISprite(2,(float)font->GetBase(),z,D3DCOLOR_XRGB(255,0,0));
	this->sprite2->SetTexturePosition(0.0f,0.0f,0.0f,0.0f);
	this->sprite->SetTexture(texture);
	this->sprite->SetTexturePosition(0.0f,0.0f,0.0f,0.0f);
	this->font = font;
	this->input = input;

	this->active = true;
	this->focused = false;
	this->caretRecoverTime = 2000;
	this->leftBraceCharPos = 0;
	this->rightBraceCharPos = 0;
	this->textBuffer = new wchar_t[64];
	for(int i = 0; i < 64; i++)
		this->textBuffer[i] = 0;
	this->textBuffer[0] = 0;
	this->normalStateCoord = new float[4];
	this->normalStateCoord[0] = this->normalStateCoord[1] = this->normalStateCoord[2] = this->normalStateCoord[3] = 0.0f;
	this->focusedStateTexCoord = NULL;
}

IGUIEditBox::~IGUIEditBox()
{
	SAFE_RELEASE(this->sprite);
	SAFE_RELEASE(this->sprite2);
	SAFE_RELEASE(this->textBuffer);
	SAFE_RELEASE(this->normalStateCoord);
	SAFE_RELEASE(this->focusedStateTexCoord);
}

void IGUIEditBox::SetNormalTextureCoord(float x0, float y0, float x1, float y1)
{
	if(this->sprite->GetTexture() == NULL)
		return;

	//получаем описание текстуры
	D3DSURFACE_DESC *surfaceDescription = new D3DSURFACE_DESC;
	this->sprite->GetTexture() ->GetLevelDesc(0,surfaceDescription);
	
	if((x1-x0 > surfaceDescription->Width) || (y1-y0 > surfaceDescription->Height))
		return;

	this->normalStateCoord[0] = x0 / surfaceDescription->Width; //x0
	this->normalStateCoord[1] = y0 / surfaceDescription->Height; //y0

	this->normalStateCoord[2] = x1 / surfaceDescription->Width; //x1
	this->normalStateCoord[3] = y1 / surfaceDescription->Height; //y1

	delete surfaceDescription;
}

void IGUIEditBox::SetFocusedTexCoord(float x0, float y0, float x1, float y1)
{
	if(this->sprite->GetTexture() == NULL)
		return;

	//получаем описание текстуры
	D3DSURFACE_DESC *surfaceDescription = new D3DSURFACE_DESC;
	this->sprite->GetTexture()->GetLevelDesc(0,surfaceDescription);
	
	if((x1-x0 > surfaceDescription->Width) || (y1-y0 > surfaceDescription->Height))
		return;

	this->focusedStateTexCoord = new float[4];
	this->focusedStateTexCoord[0] = x0 / surfaceDescription->Width; //x0
	this->focusedStateTexCoord[1] = y0 / surfaceDescription->Height; //y0

	this->focusedStateTexCoord[2] = x1 / surfaceDescription->Width; //x1
	this->focusedStateTexCoord[3] = y1 / surfaceDescription->Height; //y1

	delete surfaceDescription;
}

void IGUIEditBox::Update()
{
	if(this->active)
	{
		if(this->focused)
		{
			while((this->tempChar = this->input->GetUnicodeFromKeyQueue()) != L' ')
			{
				if(this->tempChar == 8)
				{
					if(this->rightBraceCharPos > 0)
					{
						this->rightBraceCharPos--;
						this->textBuffer[this->rightBraceCharPos] = 0;
						
						if(this->leftBraceCharPos > 0)
							this->leftBraceCharPos--;

					}
				}else{
					if(this->rightBraceCharPos < 63)
					{
						this->textBuffer[this->rightBraceCharPos] = this->tempChar;
						this->rightBraceCharPos++;
						this->textBuffer[this->rightBraceCharPos] = 0;
						while(this->font->GetStringWidth(this->leftBraceCharPos,this->rightBraceCharPos,this->textBuffer) > this->boxWidth)
						{
							this->leftBraceCharPos++;
						}
					}
				}
				if(this->tempChar == 13)
				{
					this->focused = false;
					this->sprite->SetTexturePosition(this->normalStateCoord[0],this->normalStateCoord[1],this->normalStateCoord[2],this->normalStateCoord[3]);
					break;
				}
			}
		}
		if((input->GetMouseX() >= this->boxX) && 
			(input->GetMouseY() <= this->boxY) && 
			(input->GetMouseX() <= this->boxX+this->boxWidth) && 
			(input->GetMouseY() >= this->boxY-this->boxHeight))
		{
			if(this->input->GetCurrentKeyState(VK_LBUTTON))
			{
				if(!this->focused)
				{
					this->focused = true;
					this->sprite->SetTexturePosition(this->focusedStateTexCoord[0],this->focusedStateTexCoord[1],this->focusedStateTexCoord[2],this->focusedStateTexCoord[3]);
				}
			}
		}else
		{
			if(this->input->GetCurrentKeyState(VK_LBUTTON))
				if(this->focused)
					{
						this->focused = false;
						this->sprite->SetTexturePosition(this->normalStateCoord[0],this->normalStateCoord[1],this->normalStateCoord[2],this->normalStateCoord[3]);
					}
		}
	}
}