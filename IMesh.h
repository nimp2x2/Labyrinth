#ifndef IMESH_H
#define IMESH_H

#include "ILogger.h"

struct AdvancedVertex{
	float position[3];
	float texCoord[2];
	float normal[3];
	float tangent[4];
};

struct SimpleVertex
{
	float position[3];
	float texCoord[2];
};


class IMesh
{
	SmartPointer<AdvancedVertex> *vertices;
	SmartPointer<DWORD> *indices;
	unsigned int verticesCount;
	unsigned int indicesCount;
	LPDIRECT3DTEXTURE9 texture;
	SmartPointerCOM<IDirect3DStateBlock9> *renderState;
	SmartPointer<D3DXMATRIXA16> *matWorld;
	bool isRenderable;
	bool dataFromPrev;
	ID3DXEffect *effect;
public:
	SmartPointer<AdvancedVertex>* GetVertices(){ return this->vertices; }
	SmartPointer<DWORD>* GetIndices() { return this->indices; }
	unsigned int GetIndicesCount() { return this->indicesCount; }
	unsigned int GetVerticesCount() { return this->verticesCount; }
	SmartPointerCOM<IDirect3DStateBlock9>* GetRenderStateBlock(){ return this->renderState; }
	void SetRenderStateBlock(SmartPointerCOM<IDirect3DStateBlock9> *renderState);
	LPDIRECT3DTEXTURE9 GetTexture(){ return this->texture; }
	void SetTexture(LPDIRECT3DTEXTURE9 texture) { this->texture = texture; }
	ID3DXEffect* GetEffect(){ return this->effect; }
	void SetEffect(ID3DXEffect* effect){ this->effect = effect; }
	SmartPointer<D3DXMATRIXA16>* GetWorldMatrix() { return this->matWorld; }
	void SetWorldMatrix(SmartPointer<D3DXMATRIXA16> *matWorld);
	bool IsRenderable() { return this->isRenderable; }
	void SetRenderable(bool isRendarable) { this->isRenderable = isRendarable; }
	bool GetDataFromPrev() { return this->dataFromPrev; }
	IMesh(AdvancedVertex* vertices, unsigned int verticesCount, DWORD *indices, unsigned int indicesCount, LPDIRECT3DTEXTURE9 texture, SmartPointerCOM<IDirect3DStateBlock9> *renderState, SmartPointer<D3DXMATRIXA16> *matWorld, ID3DXEffect *effect, bool isRenderable = true, bool dataFromPrev = false);
	IMesh(IMesh *mesh, bool shareData = false);
	~IMesh();
};

class ISimpleMesh
{
	SmartPointer<SimpleVertex> *vertices;
	SmartPointer<DWORD> *indices;
	unsigned int verticesCount;
	unsigned int indicesCount;
	LPDIRECT3DTEXTURE9 texture;
	SmartPointerCOM<IDirect3DStateBlock9> *renderState;
	SmartPointer<D3DXMATRIXA16> *matWorld;
	bool isRenderable;
	bool dataFromPrev;
	ID3DXEffect *effect;
public:
	SmartPointer<SimpleVertex>* GetVertices(){ return this->vertices; }
	SmartPointer<DWORD>* GetIndices() { return this->indices; }
	unsigned int GetIndicesCount() { return this->indicesCount; }
	unsigned int GetVerticesCount() { return this->verticesCount; }
	SmartPointerCOM<IDirect3DStateBlock9>* GetRenderStateBlock(){ return this->renderState; }
	void SetRenderStateBlock(SmartPointerCOM<IDirect3DStateBlock9> *renderState);
	LPDIRECT3DTEXTURE9 GetTexture(){ return this->texture; }
	void SetTexture(LPDIRECT3DTEXTURE9 texture) { this->texture = texture; }
	ID3DXEffect* GetEffect(){ return this->effect; }
	void SetEffect(ID3DXEffect* effect){ this->effect = effect; }
	SmartPointer<D3DXMATRIXA16>* GetWorldMatrix() { return this->matWorld; }
	void SetWorldMatrix(SmartPointer<D3DXMATRIXA16> *matWorld);
	bool IsRenderable() { return this->isRenderable; }
	void SetRenderable(bool isRendarable) { this->isRenderable = isRendarable; }
	bool GetDataFromPrev() { return this->dataFromPrev; }
	ISimpleMesh(SimpleVertex* vertices, unsigned int verticesCount, DWORD *indices, unsigned int indicesCount, LPDIRECT3DTEXTURE9 texture, SmartPointerCOM<IDirect3DStateBlock9> *renderState, SmartPointer<D3DXMATRIXA16> *matWorld, ID3DXEffect *effect, bool isRenderable = true, bool dataFromPrev = false);
	ISimpleMesh(ISimpleMesh *mesh, bool shareData = false);
	~ISimpleMesh();
};

#endif