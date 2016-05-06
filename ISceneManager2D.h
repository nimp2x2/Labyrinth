#ifndef ISCENEMANAGER9_H
#define ISCENEMANAGER9_H

#include "IGUI.h"

//Структура, описывающая формат вертексов для рендера с текстурой
struct Vertex2D
{
	float			x, y;		// screen position    
	float			z;			// Z-buffer depth 0..1
	DWORD			col;		// color
	float			tx, ty;		// texture coordinates
};

#define D3DFVF_Vertex2D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//формат вертексов для рендера без текстуры
struct PrimitiveVertex2D
{
	float			x, y;		// screen position    
	float			z;			// Z-buffer depth 0..1
	DWORD			col;		// color
};

#define D3DFVF_PrimitiveVertex2D (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define D3DFVF_PrimitiveVertex2D (D3DFVF_XYZ | D3DFVF_DIFFUSE)

class ISceneManager2D
{
	LPDIRECT3DVERTEXBUFFER9						vertexBuffer; //буффер вершин для  спрайтов
	LPDIRECT3DVERTEXBUFFER9						linePrimitiveVertexBuffer;//буффер вершин для 2D примитивов -  линий
	LPDIRECT3DVERTEXBUFFER9						pointPrimitiveVertexBuffer;//буффер вершин для 2D примитивов -  точек
	LPDIRECT3DINDEXBUFFER9						indexBuffer; // буффер индексов для спрайтов

	unsigned int								linePrimitivesCounter; //сколько линии нужн отрисовать?
	unsigned int								pointPrimitivesCounter; //сколько точек нужно отрисовать??
	unsigned int								spritesCount; // сколько спрайтов отрисовать?

	PrimitiveVertex2D							*lineVertexBuffer;	
	PrimitiveVertex2D							*pointVertexBuffer;	
	Vertex2D									*spriteVertexBuffer;
	LPDIRECT3DTEXTURE9							currentTexture;

	D3DXMATRIXA16								matView; //матрица просмотра
	D3DXMATRIXA16								matProj; //матрица проекции
	D3DXMATRIXA16								matWorld; //матрица мира

	LPDIRECT3DDEVICE9							D3DDevice; //указатель на устройства вывода
	IDirect3DStateBlock9*						renderStateBlock;
public:
	void										SetViewMat(D3DXVECTOR3 eyePointVector,D3DXVECTOR3 lookAtPointVector,D3DXVECTOR3 upVector); //настраиваем матрицу вида
	void										SetProjMat(float width,float height,float zNear,float zFar); // настраиваем матрицу проекции

	HRESULT										CreateVertexBuffer(unsigned int size);  //создает буффер вершин для двухмерного рендера(!!!в скобках указываем количество примитивов, а не вершин!!!)
	HRESULT										CreateIndexBuffer(unsigned int size); //создает буффер индексов для двухмерного рендера(!!!в скобках указываем количество примитивов, а не вершин!!!)
	HRESULT										CreatePointPrimitivesVertexBuffer(unsigned int size); //создает буффер вершин для 2д рендера точек(!!!в скобках указываем количество примитивов, а не вершин!!!)
	HRESULT										CreateLinePrimitivesVertexBuffer(unsigned int size); //создает буффер вершин для 2д рендера линий(!!!в скобках указываем количество примитивов, а не вершин!!!)
	
	void										DrawLine(float x,float y,float x2,float y2,float z, DWORD color); //рендер 2д линии
	void										DrawPoint(float x, float y, float z, DWORD color);// рендер 2д точки
	void										DrawRectangle(float x, float y, float width, float height, float z, DWORD color);
	void										DrawEllipse(float x, float y, float a, float b, float z, DWORD color,int nPoints = 36);
	void										DrawSprite(ISprite *sprite,float x,float y); //рендер спрайта
	void										DrawSpriteEx(ISprite *sprite,float x,float y,float angle,float horizontalScale,float verticalScale); //рендер спрайта с доп. параметрами
	void										DrawAnim(IAnim *anim,float x,float y); //рендер анимации
	void										DrawAnimEx(IAnim *anim,float x,float y,float angle,float horizontalScale,float verticalScale); //рендер анимации с доп.параметрами
	void										DrawSimpleText(IFont *font,float x,float y,int startChar, int endChar,wchar_t *format, ...); //рендер текста
	void										DrawTextEx(IFont *font,float x,float y,int align,int startChar, int endChar,wchar_t *format, ...);
	void										DrawSimpleTextInBB(IFont *font,float x,float y,float width, float height,int startChar, int endChar,wchar_t *format, ...);
	void										DrawTextExInBB(IFont *font,float x,float y,float width, float height,int align,int startChar, int endChar,wchar_t *format, ...);
	void										DrawButton(IGUIButton *button);
	void										DrawEditBox(IGUIEditBox *editBox);
	void										ReleaseAllBuffers();

	void										sceneBegin();
	void										sceneEnd(); //рендерит всю 2д графику, находящуюся в очереди

	ISceneManager2D(LPDIRECT3DDEVICE9 d3DDevice); // конструктор
	~ISceneManager2D(); // деструктор
};

#endif