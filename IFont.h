#ifndef IFONT_H
#define IFONT_H
#include "IAnim.h"

//align define
#define C2DFONT_TEXT_ALIGN_LEFT 0
#define C2DFONT_TEXT_ALIGN_CENTER 1
#define C2DFONT_TEXT_ALIGN_RIGHT 2


class FontLineQueue
{
public:
	float							startX;
	FontLineQueue					*next;
};
class CharKerning
{
public:
	int								secondCharId;
	int								amount;
};

class Char
{
public:
	int								width; // ширина буквы
	int								height; // высота буквы
	int								texXPos; //позици€ буквы в текстуре(х)
	int								texYPos; // и у 
	int								xOffset;
	int								yOffset;
	int								xAdvance;
	int								kerningCount;
	CharKerning					    *kerningTable;
};

class IFont
{
	ISprite							*sprite; // дл€ рендера букв
	Char							*charTable; // таблица символов
	int								startIndex;
	int								lastIndex;
	int								charsCount; //сколько символов в атласе?
	int								lineHeight; // высота шрифта
	int								base; // нижн€€ граница буквы
	float							scale; // масштаб
public:
	bool							AppendFontFile(wchar_t *filename);
	bool							SetTextureAtlas(LPDIRECT3DTEXTURE9 texture);
	void							SetScale(float scale);
	float							GetScale(){ return this->scale;}
	float							GetStringWidth(wchar_t *buffer);
	float							GetStringWidth(int beginChar, int endChar,wchar_t *buffer);
	ISprite*						GetSprite(){ return this->sprite; }		
	int								GetBase() { return this->base; }
	int								GetLineHeight(){ return this->lineHeight; }
	int								GetStartIndex(){ return this->startIndex; }
	Char							GetCharTable(int index){ return this->charTable[index]; }

	void							PushLineToQueue(FontLineQueue **first, FontLineQueue **last, float value);
	float							PopFromLineQueue(FontLineQueue **first, FontLineQueue **last, int index);

	IFont(); //конструктор
	~IFont();//деструктор
};

#endif