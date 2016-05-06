#include "IAnim.h"

IAnim::IAnim(float frameWidth,float frameHeight,float z,DWORD animColor,unsigned int frames,unsigned int frameRate,LPDIRECT3DTEXTURE9 texture)
{

	this->sprite = new ISprite(frameWidth,frameHeight,z,animColor,texture);
	this->frames = frames;
	currentFrame = 0;
	this->frameRate = frameRate;
	this->rateCounter = 0;
	this->play = true;
	this->textureStartXPos = textureStartYPos = 0;

	if(texture == NULL)
		return;
	D3DSURFACE_DESC *surfaceDescription = new D3DSURFACE_DESC;
	this->sprite->GetTexture()->GetLevelDesc(0,surfaceDescription);

	this->textureStartXPos = 0.0f;
	this->textureStartYPos = 0.0f;
	this->texFrameWidth = frameWidth / surfaceDescription->Width;
	this->texFrameHeight = frameHeight / surfaceDescription->Height;

	this->twoSidePlay = false;
	this->twoSideFlag= false;

	delete surfaceDescription;
}

void IAnim::SetStartTexturePositionAndSize(float x,float y,float width,float height)
{
	if(this->sprite->GetTexture()== NULL)
		return;
	D3DSURFACE_DESC *surfaceDescription = new D3DSURFACE_DESC;
	this->sprite->GetTexture()->GetLevelDesc(0,surfaceDescription);

	this->textureStartXPos = x / surfaceDescription->Width;
	this->textureStartYPos = y / surfaceDescription->Height;
	this->texFrameWidth = width / surfaceDescription->Width;
	this->texFrameHeight = height / surfaceDescription->Height;

	this->sprite->SetTexturePosition(this->textureStartXPos,this->textureStartYPos,this->texFrameWidth+this->textureStartXPos,this->texFrameHeight+this->textureStartYPos);

	delete surfaceDescription;
}

bool IAnim::Update()
{
	//включена ли смена кдаров?
	if(!this->play)
		return false;
	//прошло ли достаточное количество кадров для обновления анимации
	if(this->rateCounter > this->frameRate)
	{
		this->rateCounter = 0;
		//если текущий кадр - последний, то переходим к первому
		if(this->currentFrame == 0 && this->twoSideFlag)
			this->twoSideFlag = false;
		if(((this->currentFrame >= this->frames-1) && !this->twoSideFlag) || (((this->currentFrame == 0) && this->twoSideFlag)))
		{
			if(!this->twoSidePlay)
			{
				this->sprite->SetTexturePosition(this->textureStartXPos,this->textureStartYPos,this->texFrameWidth+this->textureStartXPos,this->texFrameHeight+this->textureStartYPos);
				this->currentFrame = 0;
			}else{
				this->twoSideFlag = true;
			}
		}else{
			float x0,y0,x1,y1;
			this->sprite->GetTexturePos(x0,y0,x1,y1);
			//если нет, то рассчитываем координаты текстуры следующего кадра
			if(!this->twoSideFlag)
			{
				x0 += this->texFrameWidth;
				y0 += this->texFrameWidth;
				if(x0+0.0001f > 1.0f)
				{
						x0 = 0.0f;
						x1 = this->texFrameWidth;
						y0 += this->texFrameHeight;
						y1 += this->texFrameHeight;
				}
				this->currentFrame++;
			}else{
				x0 -= this->texFrameWidth;
				y0 -= this->texFrameWidth;
				if(x0 < 0.0f)
				{
						x0 = 1.0f - this->texFrameWidth;
						x1 = 1.0f;
						y0 -= this->texFrameHeight;
						y1 -= this->texFrameHeight;
				}
				if(this->currentFrame > 0)
					this->currentFrame--;
			}
		}
	}else
		this->rateCounter++;
	return true;
}

//********************************************************//
//**устанавливает определенный кадр в последовательности**//
//**!! от 1 до количества кадров**************************//
//********************************************************//
void IAnim::SetCurrentFrame(unsigned int frame)
{
	if(this->frames < frame)
		this->frames = frame;

	float x0,y0,x1,y1;
	this->sprite->GetTexturePos(x0,y0,x1,y1);

	x0= this->textureStartXPos;
	y0 = this->textureStartYPos;
	x1= this->texFrameWidth+this->textureStartXPos;
	y1 = this->texFrameHeight+this->textureStartYPos;

	for(unsigned int i = 0; i < (frame-1); i++)
	{
		x0 += this->texFrameWidth;
		x1 += this->texFrameWidth;
		if(x0+0.0001f > 1.0f)
		{
			x0 = 0.0f;
			x1 = this->texFrameWidth;
			y0 += this->texFrameHeight;
			y1+= this->texFrameHeight;
		}
		this->currentFrame = frame-1;
	}
}

IAnim::~IAnim()
{
	SAFE_RELEASE(this->sprite);
}