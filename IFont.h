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
	int								width; // ������ �����
	int								height; // ������ �����
	int								texXPos; //������� ����� � ��������(�)
	int								texYPos; // � � 
	int								xOffset;
	int								yOffset;
	int								xAdvance;
	int								kerningCount;
	CharKerning					    *kerningTable;
};

class IFont
{
	ISprite							*sprite; // ��� ������� ����
	Char							*charTable; // ������� ��������
	int								startIndex;
	int								lastIndex;
	int								charsCount; //������� �������� � ������?
	int								lineHeight; // ������ ������
	int								base; // ������ ������� �����
	float							scale; // �������
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

	IFont(); //�����������
	~IFont();//����������
};

#endif