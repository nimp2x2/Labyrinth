#include "IMesh.h"

IMesh::IMesh(AdvancedVertex* vertices, unsigned int verticesCount, DWORD *indices, unsigned int indicesCount, LPDIRECT3DTEXTURE9 texture, SmartPointerCOM<IDirect3DStateBlock9> *renderState, SmartPointer<D3DXMATRIXA16> *matWorld, ID3DXEffect *effect, bool isRenderable, bool dataFromPrev)
{
	this->vertices = new SmartPointer<AdvancedVertex>(vertices, 1);
	this->verticesCount = verticesCount;
	this->indices = new SmartPointer<DWORD>(indices, 1);
	this->indicesCount = indicesCount;
	this->texture = texture;
	if(renderState != NULL)
		this->renderState = new SmartPointerCOM<IDirect3DStateBlock9>(*renderState);
	else
		this->renderState = NULL;
	if(matWorld != NULL)
		this->matWorld = new SmartPointer<D3DXMATRIXA16>(*matWorld);
	else
		this->matWorld = NULL;
	this->isRenderable = isRenderable;
	this->effect = effect;
	this->dataFromPrev = dataFromPrev;
}

IMesh::IMesh(IMesh *mesh, bool shareData)
{
	shareData ? this->vertices = NULL : this->vertices = new SmartPointer<AdvancedVertex>(*mesh->GetVertices());
	this->verticesCount = mesh->GetVerticesCount();
	shareData ? this->indices = NULL : this->indices = new SmartPointer<DWORD>(*mesh->GetIndices());
	this->indicesCount = mesh->GetIndicesCount();
	this->texture = mesh->GetTexture();
	if(mesh->GetRenderStateBlock() != NULL)
		this->renderState = new SmartPointerCOM<IDirect3DStateBlock9>(*mesh->GetRenderStateBlock());
	else
		this->renderState = NULL;
	if(mesh->GetWorldMatrix() != NULL)
		this->matWorld = new SmartPointer<D3DXMATRIXA16>(*mesh->GetWorldMatrix());
	else
		this->matWorld = NULL;
	this->isRenderable = mesh->IsRenderable();
	this->effect = mesh->GetEffect();
	this->dataFromPrev = shareData;
}

void IMesh::SetRenderStateBlock(SmartPointerCOM<IDirect3DStateBlock9> *renderState)
{
	if(this->renderState != NULL)
		delete this->renderState;
	renderState == NULL ? this->renderState = NULL : this->renderState = new SmartPointerCOM<IDirect3DStateBlock9>(*renderState);
}

void IMesh::SetWorldMatrix(SmartPointer<D3DXMATRIXA16> *matWorld)
{
	if(this->matWorld != NULL)
		delete this->matWorld;
	matWorld == NULL ? this->matWorld = NULL : this->matWorld = new SmartPointer<D3DXMATRIXA16>(*matWorld);
}

IMesh::~IMesh()
{
	SAFE_RELEASE(this->indices);
	SAFE_RELEASE(this->vertices);
	SAFE_RELEASE(this->matWorld);
	SAFE_RELEASE(this->renderState);
}

ISimpleMesh::ISimpleMesh(SimpleVertex* vertices, unsigned int verticesCount, DWORD *indices, unsigned int indicesCount, LPDIRECT3DTEXTURE9 texture, SmartPointerCOM<IDirect3DStateBlock9> *renderState, SmartPointer<D3DXMATRIXA16> *matWorld, ID3DXEffect *effect, bool isRenderable, bool dataFromPrev)
{
	this->vertices = new SmartPointer<SimpleVertex>(vertices, 1);
	this->verticesCount = verticesCount;
	this->indices = new SmartPointer<DWORD>(indices, 1);
	this->indicesCount = indicesCount;
	this->texture = texture;
	if(renderState != NULL)
		this->renderState = new SmartPointerCOM<IDirect3DStateBlock9>(*renderState);
	else
		this->renderState = NULL;
	if(matWorld != NULL)
		this->matWorld = new SmartPointer<D3DXMATRIXA16>(*matWorld);
	else
		this->matWorld = NULL;
	this->isRenderable = isRenderable;
	this->effect = effect;
	this->dataFromPrev = dataFromPrev;
}

ISimpleMesh::ISimpleMesh(ISimpleMesh *mesh, bool shareData)
{
	shareData ? this->vertices = NULL : this->vertices = new SmartPointer<SimpleVertex>(*mesh->GetVertices());
	this->verticesCount = mesh->GetVerticesCount();
	shareData ?  this->indices = NULL : this->indices = new SmartPointer<DWORD>(*mesh->GetIndices());
	this->indicesCount = mesh->GetIndicesCount();
	this->texture = mesh->GetTexture();
	if(mesh->GetRenderStateBlock() != NULL)
		this->renderState = new SmartPointerCOM<IDirect3DStateBlock9>(*mesh->GetRenderStateBlock());
	else
		this->renderState = NULL;
	if(mesh->GetWorldMatrix() != NULL)
		this->matWorld = new SmartPointer<D3DXMATRIXA16>(*mesh->GetWorldMatrix());
	else
		this->matWorld = NULL;
	this->isRenderable = mesh->IsRenderable();
	this->effect = mesh->GetEffect();
	this->dataFromPrev = shareData;
}

void ISimpleMesh::SetRenderStateBlock(SmartPointerCOM<IDirect3DStateBlock9> *renderState)
{
	if(this->renderState != NULL)
		delete this->renderState;
	renderState == NULL ? this->renderState = NULL : this->renderState = new SmartPointerCOM<IDirect3DStateBlock9>(*renderState);
}

void ISimpleMesh::SetWorldMatrix(SmartPointer<D3DXMATRIXA16> *matWorld)
{
	if(this->matWorld != NULL)
		delete this->matWorld;
	matWorld == NULL ? this->matWorld = NULL : this->matWorld = new SmartPointer<D3DXMATRIXA16>(*matWorld);
}

ISimpleMesh::~ISimpleMesh()
{
	SAFE_RELEASE(this->indices);
	SAFE_RELEASE(this->vertices);
	SAFE_RELEASE(this->matWorld);
	SAFE_RELEASE(this->renderState);
}