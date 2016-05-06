#ifndef ISCENEMANAGER9_H
#define ISCENEMANAGER9_H

#include "IGUI.h"

//���������, ����������� ������ ��������� ��� ������� � ���������
struct Vertex2D
{
	float			x, y;		// screen position    
	float			z;			// Z-buffer depth 0..1
	DWORD			col;		// color
	float			tx, ty;		// texture coordinates
};

#define D3DFVF_Vertex2D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//������ ��������� ��� ������� ��� ��������
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
	LPDIRECT3DVERTEXBUFFER9						vertexBuffer; //������ ������ ���  ��������
	LPDIRECT3DVERTEXBUFFER9						linePrimitiveVertexBuffer;//������ ������ ��� 2D ���������� -  �����
	LPDIRECT3DVERTEXBUFFER9						pointPrimitiveVertexBuffer;//������ ������ ��� 2D ���������� -  �����
	LPDIRECT3DINDEXBUFFER9						indexBuffer; // ������ �������� ��� ��������

	unsigned int								linePrimitivesCounter; //������� ����� ���� ����������?
	unsigned int								pointPrimitivesCounter; //������� ����� ����� ����������??
	unsigned int								spritesCount; // ������� �������� ����������?

	PrimitiveVertex2D							*lineVertexBuffer;	
	PrimitiveVertex2D							*pointVertexBuffer;	
	Vertex2D									*spriteVertexBuffer;
	LPDIRECT3DTEXTURE9							currentTexture;

	D3DXMATRIXA16								matView; //������� ���������
	D3DXMATRIXA16								matProj; //������� ��������
	D3DXMATRIXA16								matWorld; //������� ����

	LPDIRECT3DDEVICE9							D3DDevice; //��������� �� ���������� ������
	IDirect3DStateBlock9*						renderStateBlock;
public:
	void										SetViewMat(D3DXVECTOR3 eyePointVector,D3DXVECTOR3 lookAtPointVector,D3DXVECTOR3 upVector); //����������� ������� ����
	void										SetProjMat(float width,float height,float zNear,float zFar); // ����������� ������� ��������

	HRESULT										CreateVertexBuffer(unsigned int size);  //������� ������ ������ ��� ����������� �������(!!!� ������� ��������� ���������� ����������, � �� ������!!!)
	HRESULT										CreateIndexBuffer(unsigned int size); //������� ������ �������� ��� ����������� �������(!!!� ������� ��������� ���������� ����������, � �� ������!!!)
	HRESULT										CreatePointPrimitivesVertexBuffer(unsigned int size); //������� ������ ������ ��� 2� ������� �����(!!!� ������� ��������� ���������� ����������, � �� ������!!!)
	HRESULT										CreateLinePrimitivesVertexBuffer(unsigned int size); //������� ������ ������ ��� 2� ������� �����(!!!� ������� ��������� ���������� ����������, � �� ������!!!)
	
	void										DrawLine(float x,float y,float x2,float y2,float z, DWORD color); //������ 2� �����
	void										DrawPoint(float x, float y, float z, DWORD color);// ������ 2� �����
	void										DrawRectangle(float x, float y, float width, float height, float z, DWORD color);
	void										DrawEllipse(float x, float y, float a, float b, float z, DWORD color,int nPoints = 36);
	void										DrawSprite(ISprite *sprite,float x,float y); //������ �������
	void										DrawSpriteEx(ISprite *sprite,float x,float y,float angle,float horizontalScale,float verticalScale); //������ ������� � ���. �����������
	void										DrawAnim(IAnim *anim,float x,float y); //������ ��������
	void										DrawAnimEx(IAnim *anim,float x,float y,float angle,float horizontalScale,float verticalScale); //������ �������� � ���.�����������
	void										DrawSimpleText(IFont *font,float x,float y,int startChar, int endChar,wchar_t *format, ...); //������ ������
	void										DrawTextEx(IFont *font,float x,float y,int align,int startChar, int endChar,wchar_t *format, ...);
	void										DrawSimpleTextInBB(IFont *font,float x,float y,float width, float height,int startChar, int endChar,wchar_t *format, ...);
	void										DrawTextExInBB(IFont *font,float x,float y,float width, float height,int align,int startChar, int endChar,wchar_t *format, ...);
	void										DrawButton(IGUIButton *button);
	void										DrawEditBox(IGUIEditBox *editBox);
	void										ReleaseAllBuffers();

	void										sceneBegin();
	void										sceneEnd(); //�������� ��� 2� �������, ����������� � �������

	ISceneManager2D(LPDIRECT3DDEVICE9 d3DDevice); // �����������
	~ISceneManager2D(); // ����������
};

#endif