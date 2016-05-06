#include "IFont.h"


IFont::IFont()
{
	this->charTable = NULL;
	this->charsCount = 0;
	this->sprite = new ISprite(0.0f,0.0f,1.0f,D3DCOLOR_XRGB(255,255,255));
	this->scale = 1.0f;
}

bool IFont::AppendFontFile(wchar_t *filename)
{
	//проверяем, существует ли файл
	if(GetFileAttributes(filename) == DWORD(-1))
		return false;
	//пробуем открыть
	FILE *file;
	_wfopen_s(&file,filename,L"rb");
	if(file == NULL)
		return false;
	//проверяем файл по размеру
	fseek(file,0,SEEK_END);
	long int fileSize = 0;
	fileSize = ftell(file);
	if((fileSize % 4) != 0)
	{
		fclose(file);
		return false;
	}
	int kerningsCount = 0;
	int tempCharId = 0;
	//заполняем таблицы
	fseek(file,0,SEEK_SET);
	fread_s(&this->charsCount,sizeof(int),sizeof(int),1,file);
	fread_s(&this->startIndex,sizeof(int),sizeof(int),1,file);
	fread_s(&this->lastIndex,sizeof(int),sizeof(int),1,file);
	fread_s(&this->lineHeight,sizeof(int),sizeof(int),1,file);
	fread_s(&this->base,sizeof(int),sizeof(int),1,file);
	this->charTable = new Char[this->lastIndex-this->startIndex+1];
	for(int i = 0; i < this->lastIndex-this->startIndex+1; i++)
	{
		this->charTable[i].kerningTable = NULL;
		this->charTable[i].height = 0;
		this->charTable[i].kerningCount = 0;
		this->charTable[i].texXPos = 0;
		this->charTable[i].texYPos = 0;
		this->charTable[i].width = 0;
		this->charTable[i].xAdvance = 0;
		this->charTable[i].xOffset = 0;
		this->charTable[i].yOffset = 0;
	}
	for(int i = 0; i < this->charsCount; i++)
	{
		fread_s(&tempCharId,sizeof(int),sizeof(int),1,file);
		tempCharId -= this->startIndex;
		fread_s(&this->charTable[tempCharId].texXPos,sizeof(int),sizeof(int),1,file);
		fread_s(&this->charTable[tempCharId].texYPos,sizeof(int),sizeof(int),1,file);
		fread_s(&this->charTable[tempCharId].width,sizeof(int),sizeof(int),1,file);
		fread_s(&this->charTable[tempCharId].height,sizeof(int),sizeof(int),1,file);
		fread_s(&this->charTable[tempCharId].xOffset,sizeof(int),sizeof(int),1,file);
		fread_s(&this->charTable[tempCharId].yOffset,sizeof(int),sizeof(int),1,file);
		fread_s(&this->charTable[tempCharId].xAdvance,sizeof(int),sizeof(int),1,file);
		fread_s(&this->charTable[tempCharId].kerningCount,sizeof(int),sizeof(int),1,file);
		if(this->charTable[tempCharId].kerningCount != 0)
		{
			this->charTable[tempCharId].kerningTable = new CharKerning[this->charTable[tempCharId].kerningCount]; 
			for(int j = 0; j < this->charTable[tempCharId].kerningCount; j++)
			{
				fread_s(&this->charTable[tempCharId].kerningTable[j].secondCharId,sizeof(int),sizeof(int),1,file);
				fread_s(&this->charTable[tempCharId].kerningTable[j].amount,sizeof(int),sizeof(int),1,file);
			}
		}
		else 
			this->charTable[tempCharId].kerningTable = NULL;
	}
	fclose(file);
	return true;
}

//*******************//
//**SetTextureAtlas**//
//*******************//
bool IFont::SetTextureAtlas(LPDIRECT3DTEXTURE9 texture)
{
	//не пустой ли указатель?
	if(texture == NULL)
	{
		return false;
	}
	this->sprite->SetTexture(texture);
	return true;
}

IFont::~IFont()
{
	for(int i = 0; i < this->lastIndex-this->startIndex+1; i++)
	{
		SAFE_ARRAY_RELEASE(this->charTable[i].kerningTable);
	}
	SAFE_ARRAY_RELEASE(this->charTable);
	SAFE_RELEASE(this->sprite);
}

//************//
//**SetScale**//
//************//
void IFont::SetScale(float scale)
{
	this->scale = scale;
}

float IFont::GetStringWidth(wchar_t *buffer)
{
	int counter = 0;
	float x = 0;
	float kerning = 0.0f;

	while(buffer[counter] != 0 && counter < 255)
	{
		if(buffer[counter] == '\n')
		{
			counter++;
			continue;
		}
		if((buffer[counter]-GetStartIndex()) < 0 )
		{
			counter++;
			continue;
		}
		if(GetCharTable(buffer[counter]-GetStartIndex()).width == NULL)
		{
			counter++;
			continue;
		}

		for(int i = 0; i < GetCharTable(buffer[counter]-GetStartIndex()).kerningCount; i++)
		{
			if(GetCharTable(buffer[counter]-GetStartIndex()).kerningTable[i].secondCharId == buffer[counter+1])
			{
				kerning = (float)GetCharTable(buffer[counter]-GetStartIndex()).kerningTable[i].amount;
				break;
			}
		}
		x += GetCharTable(buffer[counter]-GetStartIndex()).xAdvance*GetScale();
		counter++;
	}
	return x;
}

float IFont::GetStringWidth(int beginChar, int endChar,wchar_t *buffer)
{
	int counter = beginChar;
	float x = 0;
	float kerning = 0.0f;
	while(buffer[counter] != 0 && counter < endChar)
	{
		if(buffer[counter] == '\n')
		{
			counter++;
			continue;
		}
		if((buffer[counter] - this->startIndex) < 0)
		{
			counter++;
			continue;
		}
		if(this->charTable[buffer[counter]-this->startIndex].kerningTable == NULL)
		{
			counter++;
			continue;
		}
		
		for(int i = 0; i < this->charTable[buffer[counter]-this->startIndex].kerningCount; i++)
		{
			if(this->charTable[buffer[counter]-this->startIndex].kerningTable[i].secondCharId == buffer[counter+1])
			{
				kerning = (float)this->charTable[buffer[counter]-this->startIndex].kerningTable[i].amount;
				break;
			}
		}
		x += (this->charTable[buffer[counter]-this->startIndex].xAdvance)*this->scale;
		counter++;
	}
	return x;
}