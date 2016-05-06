#ifndef ISCENEMANAGER_H
#define ISCENEMANAGER_H

#include "IResourceManager.h"

//render queue for static render
class StaticAdvancedRenderQueue
{
public:
	StaticAdvancedRenderQueue *next;
	IMesh *mesh;
	StaticAdvancedRenderQueue(IMesh *mesh) { this->next = NULL; this->mesh = mesh;}
};

class StaticSimpleRenderQueue
{
public:
	StaticSimpleRenderQueue *next;
	ISimpleMesh	*mesh;
	StaticSimpleRenderQueue(ISimpleMesh *mesh) { this->next = NULL; this->mesh = mesh; }
};

//scene manager
class ISceneManager
{
	//dynamic buffers
	LPDIRECT3DVERTEXBUFFER9	simpleVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 simpleIndexBuffer;
	LPDIRECT3DVERTEXBUFFER9 advancedVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 advancedIndexBuffer;
	//static buffers
	LPDIRECT3DVERTEXBUFFER9	staticSimpleVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 staticSimpleIndexBuffer;
	LPDIRECT3DVERTEXBUFFER9 staticAdvancedVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 staticAdvancedIndexBuffer;
	//static render queues
	StaticSimpleRenderQueue *staticSimpleRenderQueueBegin;
	StaticSimpleRenderQueue *staticSimpleRenderQueueEnd;
	StaticAdvancedRenderQueue *staticAdvancedRenderQueueBegin;
	StaticAdvancedRenderQueue *staticAdvancedRenderQueueEnd;
	//static render counters
	unsigned int totalStaticSimpleVerticesCount;
	unsigned int totalStaticSimpleIndicesCount;
	unsigned int totalStaticAdvancedVerticesCount;
	unsigned int totalStaticAdvancedIndicesCount;
	//dynamic render counters
	unsigned int totalSimpleVerticesCount;
	unsigned int totalSimpleIndicesCount;
	unsigned int totalAdvancedVerticesCount;
	unsigned int totalAdvancedIndicesCount;
	//render counters
	unsigned int verticesCount;
	unsigned int indicesCount;
	unsigned int pass;
	unsigned int verticesBaseIndex;
	unsigned int startIndex;
	bool checker;
	IDirect3DStateBlock9* currentStateBlock;
	D3DXMATRIXA16 *currentWorldMatrix;
	ID3DXEffect *currentEffect;
	StaticSimpleRenderQueue *tempStaticSimpleRenderQueue;
	StaticAdvancedRenderQueue *tempStaticAdvancedRenderQueue;

	SimpleVertex *staticSimpleVertices;
	DWORD *staticSimpleIndices;
	AdvancedVertex *staticAdvancedVertices;
	DWORD *staticAdvancedIndices;

	//textures
	LPDIRECT3DTEXTURE9 currentSimpleTexture;
	LPDIRECT3DTEXTURE9 currentAdvancedTexture;
	LPDIRECT3DTEXTURE9 currentDeviceTexture[8];
	//matrices
	D3DXMATRIXA16 viewMatrix;
	D3DXMATRIXA16 projectionMatrix;
	D3DXMATRIXA16 worldMatrix;
	//vertex decl
	IDirect3DVertexDeclaration9	*advancedVertexDecl;
	IDirect3DVertexDeclaration9	*simpleVertexDecl;
	//device
	LPDIRECT3DDEVICE9 D3DDevice;
public:
	void SetProjMatrix(const D3DXMATRIXA16 &newProjMatrix);
	void SetWorldMatrix(const D3DXMATRIXA16 &newWorldMatrix);	
	void SetViewMatrix(const D3DXMATRIXA16 &newViewMatrix);

	HRESULT CreateAdvancedVertexBuffer(unsigned int size);
	HRESULT	CreateAdvancedIndexBuffer(unsigned int size); 
	HRESULT	CreateSimpleVertexBuffer(unsigned int size);
	HRESULT	CreateSimpleIndexBuffer(unsigned int size); 
	HRESULT CreateStaticAdvancedVertexBuffer(unsigned int size);
	HRESULT	CreateStaticAdvancedIndexBuffer(unsigned int size); 
	HRESULT	CreateStaticSimpleVertexBuffer(unsigned int size);
	HRESULT	CreateStaticSimpleIndexBuffer(unsigned int size); 

	void StartSimpleRenderQueueFill();
	void AddStaticSimpleMesh(ISimpleMesh *mesh);
	void EndSimpleRenderQueueFill();
	void ClearSimpleRenderQueue();

	void StartAdvancedRenderQueueFill();
	void AddStaticAdvancedMesh(IMesh *mesh);
	void EndAdvancedRenderQueueFill();
	void ClearAdvancedRenderQueue();

	void SceneBegin();
	void SceneEnd();

	void ReleaseAllBuffers();

	ISceneManager(LPDIRECT3DDEVICE9 D3DDevice);
	~ISceneManager();
};

#endif
