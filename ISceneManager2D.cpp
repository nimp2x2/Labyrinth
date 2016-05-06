#include "ISceneManager2D.h"

ISceneManager2D::ISceneManager2D(LPDIRECT3DDEVICE9 d3DDevice)
{
	this->D3DDevice = d3DDevice;
	this->vertexBuffer = NULL;
	this->linePrimitiveVertexBuffer = NULL;
	this->pointPrimitiveVertexBuffer = NULL;
	this->indexBuffer = NULL;
	this->linePrimitivesCounter = 0;
	this->pointPrimitivesCounter = 0;
	this->lineVertexBuffer = NULL;
	this->pointVertexBuffer = NULL;
	this->spriteVertexBuffer = NULL;
	this->spritesCount = 0;
	D3DXMatrixIdentity(&this->matProj);
	D3DXMatrixIdentity(&this->matWorld);
	D3DXMatrixIdentity(&this->matView);
	//render state block
	this->renderStateBlock = NULL;
	/*this->D3DDevice->BeginStateBlock();
	this->D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	this->D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	this->D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2,D3DTA_TEXTURE);
	this->D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	this->D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->D3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	this->D3DDevice->EndStateBlock(&this->renderStateBlock);*/
}

ISceneManager2D::~ISceneManager2D()
{
	SAFE_COM_RELEASE(this->renderStateBlock);
	SAFE_COM_RELEASE(this->vertexBuffer);
	SAFE_COM_RELEASE(this->linePrimitiveVertexBuffer);
	SAFE_COM_RELEASE(this->pointPrimitiveVertexBuffer);
	SAFE_COM_RELEASE(this->indexBuffer);
}

void ISceneManager2D::SetProjMat(float width,float height,float zNear,float zFar)
{
	D3DXMatrixOrthoOffCenterLH(&this->matProj,0.0f,width,0.0f,height, zNear, zFar);
}

void ISceneManager2D::SetViewMat(D3DXVECTOR3 eyePointVector,D3DXVECTOR3 lookAtPointVector,D3DXVECTOR3 upVector)
{
	D3DXMatrixLookAtLH(&this->matView,&eyePointVector,&lookAtPointVector,&upVector);
}

HRESULT ISceneManager2D::CreateIndexBuffer(unsigned int size)
{
	HRESULT hr =  this->D3DDevice->CreateIndexBuffer(size * sizeof(DWORD)*6,D3DUSAGE_WRITEONLY,D3DFMT_INDEX32,D3DPOOL_DEFAULT,&this->indexBuffer,NULL);
	//если не удалось, выходим
	if(FAILED(hr))
		return hr;
	//заполняем
	DWORD* indices;
	this->indexBuffer->Lock(0,0,(void**)&indices, 0);
	int counter = 0;
	int primitiveCount = 0;
	for(unsigned int i = 0; i < size*6; i++)
	{
		switch (counter)
		{
		case 0: 
			indices[i] = primitiveCount*4;
			counter++;
			break;
		case 1:
			indices[i] = primitiveCount*4+3;
			counter++;
			break;
		case 2:
			indices[i] = primitiveCount*4+2;
			counter++;
			break;
		case 3:
			indices[i] = primitiveCount*4;
			counter++;
			break;
		case 4:
			indices[i] = primitiveCount*4+1;
			counter++;
			break;
		case 5:
			indices[i] = primitiveCount*4+3;
			counter = 0;
			primitiveCount++;
			break;
		}
	}
	this->indexBuffer->Unlock();
	return hr;
}

HRESULT ISceneManager2D::CreateLinePrimitivesVertexBuffer(unsigned int size)
{
	return this->D3DDevice->CreateVertexBuffer(2 * size * sizeof(PrimitiveVertex2D),D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,D3DFVF_PrimitiveVertex2D,D3DPOOL_DEFAULT,&this->linePrimitiveVertexBuffer,NULL);
}

HRESULT ISceneManager2D::CreatePointPrimitivesVertexBuffer(unsigned int size)
{
	return this->D3DDevice->CreateVertexBuffer(size * sizeof(PrimitiveVertex2D),D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,D3DFVF_PrimitiveVertex2D,D3DPOOL_DEFAULT,&this->pointPrimitiveVertexBuffer,NULL);
}

HRESULT ISceneManager2D::CreateVertexBuffer(unsigned int size)
{
	return D3DDevice->CreateVertexBuffer(size * sizeof(Vertex2D)*4,D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,D3DFVF_Vertex2D,D3DPOOL_DEFAULT,&this->vertexBuffer,NULL);
}

void ISceneManager2D::DrawLine(float x,float y,float x2,float y2,float z, DWORD color)
{
	PrimitiveVertex2D lineVertices[2];
	lineVertices[0].x = x;
	lineVertices[0].y = y;
	lineVertices[0].z = z;
	lineVertices[0].col = color;

	lineVertices[1].x = x2;
	lineVertices[1].y = y2;		
	lineVertices[1].z = z;
	lineVertices[1].col = color;

	if(this->linePrimitivesCounter == 0)
	{
		this->linePrimitiveVertexBuffer->Lock(0, 0, (void**)&this->lineVertexBuffer,D3DLOCK_DISCARD);
		memcpy(this->lineVertexBuffer,lineVertices,sizeof(PrimitiveVertex2D)*2);

	}else{
		memcpy(this->lineVertexBuffer + this->linePrimitivesCounter*2 ,&lineVertices,sizeof(PrimitiveVertex2D)*2);
	}
	this->linePrimitivesCounter++;
}

void ISceneManager2D::DrawPoint(float x, float y, float z, DWORD color)
{
	PrimitiveVertex2D pointVertices;
	pointVertices.x = x;
	pointVertices.y = y;
	pointVertices.z = z;
	pointVertices.col = color;
	
	if(this->pointPrimitivesCounter == 0)
	{
		this->pointPrimitiveVertexBuffer->Lock(0, 0, (void**)&this->pointVertexBuffer,D3DLOCK_DISCARD);
		memcpy(this->pointVertexBuffer,&pointVertices,sizeof(PrimitiveVertex2D));
	}else
	{
		memcpy(this->pointVertexBuffer + this->pointPrimitivesCounter ,&pointVertices,sizeof(PrimitiveVertex2D));
	}
	this->pointPrimitivesCounter++;
}

void ISceneManager2D::DrawSprite(ISprite *sprite,float x,float y)
{
	Vertex2D vertices[4];
	float tex0, tex1, tex2, tex3;
	sprite->GetTexturePos(tex0,tex1,tex2,tex3);

	vertices[0].x = x;
	vertices[0].y = y;
	vertices[0].col = sprite->GetColor();
	vertices[0].z = sprite->GetZ();
	vertices[0].tx = tex0;
	vertices[0].ty = tex1;

	vertices[1].x = x+sprite->GetWidth();
	vertices[1].y = y;
	vertices[1].col = sprite->GetColor();
	vertices[1].z = sprite->GetZ();
	vertices[1].tx = tex2;
	vertices[1].ty = tex1;

	vertices[2].x = x;
	vertices[2].y = y-sprite->GetHeight();
	vertices[2].col = sprite->GetColor();
	vertices[2].z = sprite->GetZ();
	vertices[2].tx = tex0;
	vertices[2].ty = tex3;

	vertices[3].x = x + sprite->GetWidth();
	vertices[3].y = y - sprite->GetHeight();
	vertices[3].col = sprite->GetColor();
	vertices[3].z = sprite->GetZ();
	vertices[3].tx = tex2;
	vertices[3].ty = tex3;

	if(this->spritesCount == 0)
	{
		this->vertexBuffer->Lock(0, 0, (void**)&this->spriteVertexBuffer,D3DLOCK_DISCARD);
		memcpy(this->spriteVertexBuffer,vertices,sizeof(Vertex2D)*4);
		this->currentTexture = sprite->GetTexture();
		this->spritesCount++;
	}else{
		if(this->currentTexture == sprite->GetTexture())
		{
			memcpy(this->spriteVertexBuffer + this->spritesCount*4,vertices,sizeof(Vertex2D)*4);
			this->spritesCount++;
		}else{
			this->vertexBuffer->Unlock();
			this->D3DDevice->SetIndices(this->indexBuffer);
			this->D3DDevice->SetFVF(D3DFVF_Vertex2D);
			this->D3DDevice->SetStreamSource(0,this->vertexBuffer, 0,sizeof(Vertex2D));
			this->D3DDevice->SetTexture(0,this->currentTexture);
			this->D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,this->spritesCount*4,0,this->spritesCount*2);
			this->vertexBuffer->Lock(0, 0, (void**)&this->spriteVertexBuffer,D3DLOCK_DISCARD);
			memcpy(this->spriteVertexBuffer,vertices,sizeof(Vertex2D)*4);
			this->currentTexture = sprite->GetTexture();
			this->spritesCount = 1;
		}
	}
}

void ISceneManager2D::DrawSpriteEx(ISprite *sprite,float x,float y,float angle,float horizontalScale,float verticalScale)
{
	Vertex2D vertices[4];
	float tex0,tex1,tex2,tex3;
	sprite->GetTexturePos(tex0,tex1,tex2,tex3);
	if(angle != 0.0f)
	{
		float tx1, ty1, tx2, ty2;
		float sint, cost;
		float hotX, hotY;
		
		sprite->GetHotSpot(hotX,hotY);

		tx1 = -hotX*horizontalScale;
		ty1 = -hotY*verticalScale;
		tx2 = (sprite->GetWidth()- hotX)*horizontalScale;
		ty2 = (sprite->GetHeight() - hotY)*verticalScale;
		angle = angle * 3.14f / 180.f;
		
		cost = cosf(angle);
		sint = sinf(angle);

		vertices[0].x = tx1*cost - ty2*sint + x + hotX*horizontalScale;
		vertices[0].y = tx1*sint + ty2*cost + y + hotY*verticalScale;
		vertices[0].col = sprite->GetColor();
		vertices[0].z = sprite->GetZ();
		vertices[0].tx = tex0;
		vertices[0].ty = tex1;

		vertices[1].x = tx2*cost - ty2*sint + x + hotX*horizontalScale;
		vertices[1].y = tx2*sint + ty2*cost + y + hotY*verticalScale;
		vertices[1].col = sprite->GetColor();
		vertices[1].z = sprite->GetZ();
		vertices[1].tx = tex2;
		vertices[1].ty = tex1;

		vertices[2].x = tx1*cost - ty1*sint + x + hotX*horizontalScale;
		vertices[2].y = tx1*sint + ty1*cost + y + hotY*verticalScale;
		vertices[2].col = sprite->GetColor();
		vertices[2].z = sprite->GetZ();
		vertices[2].tx = tex0;
		vertices[2].ty = tex3;

		vertices[3].x = tx2*cost - ty1*sint + x + hotX*horizontalScale;
		vertices[3].y = tx2*sint + ty1*cost + y + hotY*verticalScale;
		vertices[3].col = sprite->GetColor();
		vertices[3].z = sprite->GetZ();
		vertices[3].tx = tex2;
		vertices[3].ty = tex3;
	}else{
		vertices[0].x = x;
		vertices[0].y = y;
		vertices[0].col = sprite->GetColor();
		vertices[0].z = sprite->GetZ();
		vertices[0].tx = tex0;
		vertices[0].ty = tex1;

		vertices[1].x = x+sprite->GetWidth()*horizontalScale;
		vertices[1].y = y;
		vertices[1].col = sprite->GetColor();
		vertices[1].z = sprite->GetZ();
		vertices[1].tx = tex2;
		vertices[1].ty = tex1;

		vertices[2].x = x;
		vertices[2].y = y-sprite->GetHeight()*verticalScale;
		vertices[2].col = sprite->GetColor();;
		vertices[2].z = sprite->GetZ();
		vertices[2].tx = tex0;
		vertices[2].ty = tex3;

		vertices[3].x = x+sprite->GetWidth()*horizontalScale;
		vertices[3].y = y-sprite->GetHeight()*verticalScale;
		vertices[3].col = sprite->GetColor();
		vertices[3].z = sprite->GetZ();
		vertices[3].tx = tex2;
		vertices[3].ty = tex3;
	}
	if(this->spritesCount == 0)
	{
		this->vertexBuffer->Lock(0, 0, (void**)&this->spriteVertexBuffer,D3DLOCK_DISCARD);
		memcpy(this->spriteVertexBuffer,vertices,sizeof(Vertex2D)*4);
		this->currentTexture = sprite->GetTexture();
		this->spritesCount++;
	}else{
		if(this->currentTexture == sprite->GetTexture())
		{
			memcpy(this->spriteVertexBuffer + this->spritesCount*4,vertices,sizeof(Vertex2D)*4);
			this->spritesCount++;
		}else{
			this->vertexBuffer->Unlock();
			this->D3DDevice->SetIndices(this->indexBuffer);
			this->D3DDevice->SetFVF(D3DFVF_Vertex2D);
			this->D3DDevice->SetStreamSource(0,this->vertexBuffer, 0,sizeof(Vertex2D));
			this->D3DDevice->SetTexture(0,this->currentTexture);
			this->D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,this->spritesCount*4,0,this->spritesCount*2);
			this->vertexBuffer->Lock(0, 0, (void**)&this->spriteVertexBuffer,D3DLOCK_DISCARD);
			memcpy(this->spriteVertexBuffer,vertices,sizeof(Vertex2D)*4);
			this->currentTexture = sprite->GetTexture();
			this->spritesCount = 1;
		}
	}
}

void ISceneManager2D::DrawAnim(IAnim *anim,float x,float y)
{
	this->DrawSprite(anim->GetSprite(),x,y);
}

void ISceneManager2D::DrawAnimEx(IAnim *anim,float x,float y,float angle,float horizontalScale,float verticalScale)
{
	DrawSpriteEx(anim->GetSprite(),x,y,angle,horizontalScale,verticalScale);
}

void ISceneManager2D::DrawSimpleText(IFont *font,float x,float y,int startChar, int endChar,wchar_t *format, ...)
{
	wchar_t buffer[256]; //буффер для хранения форматированной строки
	va_list args; //список аргументов
	va_start (args, format); //устанавливаеам маркер на первую переменную
	vswprintf_s (buffer,256,format, args); //форматируем строку
	va_end (args); //завершаем работу со списком

	int counter = startChar;
	float kerning = 0;
	float startX = x;
	while(buffer[counter] != 0 && counter < endChar)
	{
		if(buffer[counter] == '\n')
		{
			y -= font->GetLineHeight()*font->GetScale();
			x = startX;
			counter++;
			continue;
		}
		if((buffer[counter]-font->GetStartIndex()) < 0 )
		{
			counter++;
			continue;
		}
		if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).width == NULL)
		{
			counter++;
			continue;
		}
		font->GetSprite()->SetTexturePosition((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texXPos,
			(float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texYPos,
			(float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texXPos+font->GetCharTable(buffer[counter]-font->GetStartIndex()).width,
			(float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texYPos+font->GetCharTable(buffer[counter]-font->GetStartIndex()).height);

		font->GetSprite()->SetWidth((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).width*font->GetScale());
		font->GetSprite()->SetHeight((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).height*font->GetScale());
		for(int i = 0; i < font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningCount; i++)
		{
			if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].secondCharId == buffer[counter+1])
			{
				kerning = (float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].amount;
				break;
			}
		}
		this->DrawSprite(font->GetSprite(),x+(font->GetCharTable(buffer[counter]-font->GetStartIndex()).xOffset+kerning)*font->GetScale(),y-(font->GetCharTable(buffer[counter]-font->GetStartIndex()).yOffset)*font->GetScale());
		x += font->GetCharTable(buffer[counter]-font->GetStartIndex()).xAdvance*font->GetScale();
		counter++;
	}
}

void ISceneManager2D::sceneEnd()
{	
	if(this->spritesCount > 0)
	{
		this->vertexBuffer->Unlock();
		this->D3DDevice->SetIndices(this->indexBuffer);
		this->D3DDevice->SetFVF(D3DFVF_Vertex2D);
		this->D3DDevice->SetStreamSource(0,this->vertexBuffer, 0,sizeof(Vertex2D));
		this->D3DDevice->SetTexture(0,this->currentTexture);
		this->D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,this->spritesCount*4,0,this->spritesCount*2);
		this->spritesCount = 0;
	}

	if(this->linePrimitivesCounter > 0)
	{
		this->linePrimitiveVertexBuffer->Unlock();
		this->D3DDevice->SetFVF(D3DFVF_PrimitiveVertex2D);
		this->D3DDevice->SetStreamSource(0,this->linePrimitiveVertexBuffer, 0,sizeof(PrimitiveVertex2D));
		this->D3DDevice->SetTexture(0,NULL);
		this->D3DDevice->DrawPrimitive(D3DPT_LINELIST,0,this->linePrimitivesCounter);
		this->linePrimitivesCounter = 0;
	}

	if(this->pointPrimitivesCounter > 0)
	{
		this->pointPrimitiveVertexBuffer->Unlock();
		this->D3DDevice->SetFVF(D3DFVF_PrimitiveVertex2D);
		this->D3DDevice->SetTexture(0,NULL);
		this->D3DDevice->SetStreamSource(0,this->pointPrimitiveVertexBuffer, 0,sizeof(PrimitiveVertex2D));
		this->D3DDevice->DrawPrimitive(D3DPT_POINTLIST,0,this->pointPrimitivesCounter);
		this->pointPrimitivesCounter = 0;
	}
}

void ISceneManager2D::DrawTextEx(IFont *font,float x,float y,int align,int startChar, int endChar,wchar_t *format, ...)
{
	wchar_t buffer[256]; 
	va_list args; 
	va_start (args, format); 
	vswprintf_s (buffer,256,format, args); 
	va_end (args); 

	int counter = startChar;
	float kerning = 0;
	float startX = x;
	FontLineQueue *first = NULL;
	FontLineQueue *last = NULL;
	float xOffset = 0;
	float maxXOffset = 0;

	while(buffer[counter] != 0 && counter < endChar && align != 0)
	{
		if(buffer[counter] == '\n')
		{
			if(first == NULL)
			{
				first = new FontLineQueue;
				first->next = NULL;
				first->startX  = xOffset;
				last = first;
			}else{
				last->next = new FontLineQueue;
				last->next->next = NULL;
				last->next->startX = xOffset;
				last = last->next;
			}
			xOffset = 0;
			counter++;
			continue;
		}
		if((buffer[counter]-font->GetStartIndex()) < 0 )
		{
			counter++;
			continue;
		}
		if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).width == 0)
		{
			counter++;
			continue;
		}
		kerning = 0;
		for(int i = 0; i < font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningCount; i++)
		{
			if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].secondCharId == buffer[counter+1])
			{
				kerning = font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].amount*font->GetScale();
				break;
			}
		}
		xOffset += font->GetCharTable(buffer[counter]-font->GetStartIndex()).xAdvance*font->GetScale() + kerning;
		counter++;
	}

	if(align != 0)
	{
		if(first == NULL)
		{
			first = new FontLineQueue;
			first->next = NULL;
			first->startX  = xOffset;
			last = first;
		}else{
			last->next = new FontLineQueue;
			last->next->next = NULL;
			last->next->startX = xOffset;
			last = last->next;
		}
		if(align == 2)
			x = x - first->startX;
		else
			x = x - first->startX / 2;
		last = first;
		first = first->next;
		delete last;
	}

	kerning = 0;
	counter = startChar;

	while(buffer[counter] != 0 && counter < endChar)
	{
		if(buffer[counter] == '\n')
		{
			y -= font->GetLineHeight()*font->GetScale();
			if(align != 0)
			{
				if(align == 2)
					x = startX - first->startX;
				else
					x = startX - first->startX/2;
				last = first->next;
				delete first;
				first = last;
			}else
				x = startX;
			counter++;
			continue;
		}
		if((buffer[counter]-font->GetStartIndex()) < 0 )
		{
			counter++;
			continue;
		}
		if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).width == 0)
		{
			counter++;
			continue;
		}
		font->GetSprite()->SetTexturePosition((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texXPos,
			(float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texYPos,
			(float)(font->GetCharTable(buffer[counter]-font->GetStartIndex()).texXPos + font->GetCharTable(buffer[counter]-font->GetStartIndex()).width),
			(float)(font->GetCharTable(buffer[counter]-font->GetStartIndex()).texYPos + font->GetCharTable(buffer[counter]-font->GetStartIndex()).height));
		font->GetSprite()->SetWidth((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).width*font->GetScale());
		font->GetSprite()->SetHeight((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).height*font->GetScale());
		for(int i = 0; i < font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningCount; i++)
		{
			if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].secondCharId == buffer[counter+1])
			{
				kerning = font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].amount*font->GetScale();;
				break;
			}
		}
		this->DrawSprite(font->GetSprite(),x+font->GetCharTable(buffer[counter]-font->GetStartIndex()).xOffset*font->GetScale()+kerning,y-font->GetCharTable(buffer[counter]-font->GetStartIndex()).yOffset*font->GetScale());
		x += font->GetCharTable(buffer[counter]-font->GetStartIndex()).xAdvance*font->GetScale()+kerning;
		counter++;
	}
}

void ISceneManager2D::DrawSimpleTextInBB(IFont *font,float x,float y,float width, float height,int startChar, int endChar,wchar_t *format, ...)
{
	wchar_t buffer[256];
	va_list args;
	va_start (args, format);
	vswprintf_s (buffer,256,format, args);
	va_end (args);

	int counter = startChar;
	if((buffer[counter] - font->GetStartIndex()) > 0)
		if(buffer[counter] != '\n')
			if((x+font->GetCharTable(buffer[counter]-font->GetStartIndex()).xAdvance*font->GetScale() > x+width) || (y-font->GetLineHeight() < y - height))
				return;
	float kerning = 0;
	float startX = x;
	float startY = y;
	while(buffer[counter] != 0 && counter < endChar)
	{
		if(buffer[counter] == '\n')
		{
			if((y-font->GetLineHeight()*font->GetScale()*2) < startY-height)
				return;
			y -= font->GetLineHeight()*font->GetScale();
			x = startX;
			counter++;
			continue;
		}
		if((buffer[counter]-font->GetStartIndex()) < 0 )
		{
			counter++;
			continue;
		}
		if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).width == 0)
		{
			counter++;
			continue;
		}
		font->GetSprite()->SetTexturePosition((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texXPos,
			(float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texYPos,
			(float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texXPos+ font->GetCharTable(buffer[counter]-font->GetStartIndex()).width,
			(float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texYPos+ font->GetCharTable(buffer[counter]-font->GetStartIndex()).height);
		font->GetSprite()->SetWidth((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).width*font->GetScale());
		font->GetSprite()->SetHeight((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).height*font->GetScale());
		for(int i = 0; i < font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningCount; i++)
		{
			if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].secondCharId == buffer[counter+1])
			{
				kerning = font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].amount*font->GetScale();
				break;
			}
		}
		if(x+font->GetCharTable(buffer[counter]-font->GetStartIndex()).xAdvance*font->GetScale()+kerning+font->GetCharTable(buffer[counter]-font->GetStartIndex()).xOffset > startX+width)
		{
			y -= font->GetLineHeight()*font->GetScale();
			x = startX;
			if(y-font->GetLineHeight()*font->GetScale() < startY-height)
				return;
		}
		this->DrawSprite(font->GetSprite(),x+font->GetCharTable(buffer[counter]-font->GetStartIndex()).xOffset*font->GetScale()+kerning,y-font->GetCharTable(buffer[counter]-font->GetStartIndex()).yOffset*font->GetScale());
		x += font->GetCharTable(buffer[counter]-font->GetStartIndex()).xAdvance*font->GetScale()+kerning;
		counter++;
	}
}

void ISceneManager2D::DrawTextExInBB(IFont *font,float x,float y,float width, float height,int align,int startChar, int endChar,wchar_t *format, ...)
{
	wchar_t buffer[256]; 
	va_list args;
	va_start (args, format); 
	vswprintf_s (buffer,256,format, args);
	va_end (args);

	int counter = startChar;
	while(font->GetCharTable(buffer[counter]-font->GetStartIndex()).width == 0 && counter < 256 && buffer[counter] != 0)
		counter++;
	if(counter > endChar || buffer[counter] == 0)
		return;
	if(buffer[counter] != '\n')
		if((x+font->GetCharTable(buffer[counter]-font->GetStartIndex()).xAdvance*font->GetScale() > x+width) || (y-font->GetLineHeight() < y - height))
			return;
	counter = startChar;
	float kerning = 0;
	float startX = x;
	FontLineQueue *first = NULL;
	FontLineQueue *last = NULL;
	float xOffset = 0;
	float maxXOffset = 0;
	float startY = y;
	float startXForCompare = x;

	while(buffer[counter] != 0 && counter < endChar && align != 0)
	{
		if(buffer[counter] == '\n')
		{
			if(y-font->GetLineHeight()*font->GetScale()*2 < startY-height)
				break;
			y -= font->GetLineHeight()*font->GetScale();
			if(first == NULL)
			{
				first = new FontLineQueue;
				first->next = NULL;
				first->startX  = xOffset;
				last = first;
			}else{
				last->next = new FontLineQueue;
				last->next->next = NULL;
				last->next->startX = xOffset;
				last = last->next;
			}
			xOffset = 0;
			counter++;
			continue;
		}
		if((buffer[counter]-font->GetStartIndex()) < 0 )
		{
			counter++;
			continue;
		}
		if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).width == 0)
		{
			counter++;
			continue;
		}
		
		kerning = 0;
		for(int i = 0; i < font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningCount; i++)
		{
			if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].secondCharId == buffer[counter+1])
			{
				kerning = font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].amount*font->GetScale();
				break;
			}
		}
		if(x+xOffset+kerning+font->GetCharTable(buffer[counter]-font->GetStartIndex()).xAdvance*font->GetScale() > x+width)
		{
			if(y-font->GetLineHeight()*font->GetScale()*2 < startY-height)
				break;
			y -= font->GetLineHeight()*font->GetScale();
			if(first == NULL)
			{
				first = new FontLineQueue;
				first->next = NULL;
				first->startX  = xOffset;
				last = first;
			}else{
				last->next = new FontLineQueue;
				last->next->next = NULL;
				last->next->startX = xOffset;
				last = last->next;
			}
			xOffset = 0;
		}
		xOffset += font->GetCharTable(buffer[counter]-font->GetStartIndex()).xAdvance*font->GetScale() + kerning;
		counter++;
	}

	if(align != 0)
	{
		if(first == NULL)
		{
			first = new FontLineQueue;
			first->next = NULL;
			first->startX  = xOffset;
			last = first;
		}else{
			last->next = new FontLineQueue;
			last->next->next = NULL;
			last->next->startX = xOffset;
			last = last->next;
		}
		if(align == 2)
			x = x - first->startX;
		else
			x = x - first->startX / 2;
		last = first;
		first = first->next;
		delete last;
	}

	kerning = 0;
	counter = startChar;
	y = startY;
	
	if(align != 0)
		startXForCompare=x;

	while(buffer[counter] != 0 && counter < 256)
	{
		if(buffer[counter] == '\n')
		{
			if(y-font->GetLineHeight()*font->GetScale()*2 < startY-height)
				return;
			if(align != 0)
			{
				if(align == 2)
					x = startX - first->startX;
				else
					x = startX - first->startX/2;
				last = first->next;
				delete first;
				first = last;
			}else
				x = startX;
			y -= font->GetLineHeight()*font->GetScale();;
			startXForCompare=x;
			counter++;
			continue;
		}
		if((buffer[counter]-font->GetStartIndex()) < 0 )
		{
			counter++;
			continue;
		}
		if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).width == 0)
		{
			counter++;
			continue;
		}
		font->GetSprite()->SetTexturePosition((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texXPos,
			(float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).texYPos,
			(float)(font->GetCharTable(buffer[counter]-font->GetStartIndex()).texXPos+font->GetCharTable(buffer[counter]-font->GetStartIndex()).width),
			(float)(font->GetCharTable(buffer[counter]-font->GetStartIndex()).texYPos+font->GetCharTable(buffer[counter]-font->GetStartIndex()).height));
		font->GetSprite()->SetWidth((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).width*font->GetScale());
		font->GetSprite()->SetHeight((float)font->GetCharTable(buffer[counter]-font->GetStartIndex()).height*font->GetScale());
		for(int i = 0; i < font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningCount; i++)
		{
			if(font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].secondCharId == buffer[counter+1])
			{
				kerning = font->GetCharTable(buffer[counter]-font->GetStartIndex()).kerningTable[i].amount*font->GetScale();
				break;
			}
		}
		if((x+font->GetCharTable(buffer[counter]-font->GetStartIndex()).xAdvance*font->GetScale()+kerning) > (startXForCompare+width))
		{
			if(y-font->GetLineHeight()*font->GetScale()*2 < startY-height)
				return;
			if(align != 0)
			{
				if(align == 2)
					x = startX - first->startX;
				else
					x = startX - first->startX/2;
				last = first->next;
				delete first;
				first = last;
			}else
				x = startX;
			startXForCompare=x;
			y -= font->GetLineHeight()*font->GetScale();
		}
		this->DrawSprite(font->GetSprite(),
			x+font->GetCharTable(buffer[counter]-font->GetStartIndex()).xOffset*font->GetScale()+kerning,
			y-font->GetCharTable(buffer[counter]-font->GetStartIndex()).yOffset*font->GetScale());
		x += font->GetCharTable(buffer[counter]-font->GetStartIndex()).xAdvance*font->GetScale()+kerning;
		counter++;
	}
}

void ISceneManager2D::DrawRectangle(float x, float y, float width, float height, float z, DWORD color)
{
	this->DrawLine(x,y,x+width,y,z,color);
	this->DrawLine(x,y,x,y-height,z,color);
	this->DrawLine(x+width,y,x+width,y-height,z,color);
	this->DrawLine(x,y-height,x+width,y-height,z,color);
}

void ISceneManager2D::DrawEllipse(float x, float y, float a, float b, float z, DWORD color, int points)
{
	float x0 = x+a, y0 = y;
	float x1,y1;
	float wedgeAngle = 6.28f/points;
	int i = 0;
	while(i <= points)
	{
		x1 = a * cosf(i*wedgeAngle)+x;
		y1 = y - b * sinf(i*wedgeAngle); 
		this->DrawLine(x0,y0,x1,y1,z,color);
		x0 = x1;
		y0 = y1;
		i++;
	}
}

void ISceneManager2D::DrawButton(IGUIButton *button)
{
	this->DrawSprite(button->GetSprite(),button->GetButtonX(),button->GetButtonX());
}

void ISceneManager2D::DrawEditBox(IGUIEditBox *editBox)
{
	this->DrawSprite(editBox->GetSprite(),editBox->GetEditX(),editBox->GetEditY());
	if((editBox->GetFocusState() && timeGetTime() % editBox->GetCarretRecoverTime() > (DWORD)editBox->GetCarretRecoverTime()/2))
		this->DrawSprite(editBox->GetSprite(1),editBox->GetEditX()+1+editBox->GetFont()->GetStringWidth(editBox->GetLeftBracePos(),editBox->GetRightBracePos(),editBox->GetString()),editBox->GetEditY()-(editBox->GetFont()->GetLineHeight()-editBox->GetFont()->GetBase())/2.0f);
	this->DrawSimpleTextInBB(editBox->GetFont(),editBox->GetEditX()+2,editBox->GetEditY(),editBox->GetEditWidth(),editBox->GetEditHeight(),editBox->GetLeftBracePos(),editBox->GetRightBracePos(),editBox->GetString());
}

void ISceneManager2D::sceneBegin()
{
	this->D3DDevice->SetTransform( D3DTS_PROJECTION, &this->matProj );
	this->D3DDevice->SetTransform( D3DTS_VIEW, &this->matView );
	this->D3DDevice->SetTransform( D3DTS_WORLD, &this->matWorld );
	if(this->renderStateBlock != NULL)
		this->renderStateBlock->Apply();
}

void ISceneManager2D::ReleaseAllBuffers()
{
	SAFE_COM_RELEASE(this->renderStateBlock);
	SAFE_COM_RELEASE(this->vertexBuffer);
	SAFE_COM_RELEASE(this->linePrimitiveVertexBuffer);
	SAFE_COM_RELEASE(this->pointPrimitiveVertexBuffer);
	SAFE_COM_RELEASE(this->indexBuffer);
}