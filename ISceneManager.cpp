#include "ISceneManager.h"

ISceneManager::ISceneManager(LPDIRECT3DDEVICE9 D3DDevice)
{
	//d3dvertex declaration
	D3DVERTEXELEMENT9 simpleVertexDeclaration[]=
	{
		{0,0, D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
		{0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
		D3DDECL_END()
	};


	D3DVERTEXELEMENT9 advancedVertexDeclaration[]=
	{
		{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
		{0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
		D3DDECL_END()
	};
	this->D3DDevice = D3DDevice;
	this->D3DDevice->CreateVertexDeclaration(advancedVertexDeclaration,&this->advancedVertexDecl);
	this->D3DDevice->CreateVertexDeclaration(simpleVertexDeclaration,&this->simpleVertexDecl);
	this->currentSimpleTexture = NULL;
	this->currentAdvancedTexture = NULL;
	for(int i = 0; i < 8; i++) 
		this->currentDeviceTexture[i] = NULL;
	this->advancedIndexBuffer = NULL;
	this->advancedVertexBuffer = NULL;
	this->simpleIndexBuffer = NULL;
	this->simpleVertexBuffer = NULL;
	this->staticAdvancedIndexBuffer = NULL;
	this->staticAdvancedVertexBuffer = NULL;
	this->staticSimpleIndexBuffer = NULL;
	this->staticSimpleVertexBuffer = NULL;
	this->totalStaticSimpleVerticesCount = 0;
	this->totalStaticSimpleIndicesCount = 0;
	this->totalStaticAdvancedVerticesCount = 0;
	this->totalStaticAdvancedIndicesCount = 0;
	this->totalSimpleVerticesCount = 0;
	this->totalSimpleIndicesCount = 0;
	this->totalAdvancedVerticesCount = 0;
	this->totalAdvancedIndicesCount = 0;
	this->verticesCount = 0;
	this->indicesCount = 0;
	this->pass = 0;
	this->currentStateBlock = NULL;
	this->tempStaticAdvancedRenderQueue = NULL;
	this->tempStaticSimpleRenderQueue = NULL;
	this->staticSimpleRenderQueueBegin = NULL;
	this->staticSimpleRenderQueueEnd = NULL;
	this->staticAdvancedRenderQueueBegin = NULL;
	this->staticAdvancedRenderQueueEnd = NULL;
	this->staticSimpleVertices = NULL;
	this->staticSimpleIndices = NULL;
	this->staticAdvancedVertices = NULL;
	this->staticAdvancedIndices = NULL;
	this->currentEffect = NULL;
	this->currentWorldMatrix = NULL;
	D3DXMatrixIdentity(&this->viewMatrix);
	D3DXMatrixIdentity(&this->projectionMatrix);
	D3DXMatrixIdentity(&this->worldMatrix);
}

ISceneManager::~ISceneManager()
{
	while(this->staticSimpleRenderQueueBegin != NULL)
	{
		this->staticSimpleRenderQueueEnd = this->staticSimpleRenderQueueBegin->next;
		delete this->staticSimpleRenderQueueBegin;
		this->staticSimpleRenderQueueBegin = this->staticSimpleRenderQueueEnd;
	}
	while(this->staticAdvancedRenderQueueBegin != NULL)
	{
		this->staticAdvancedRenderQueueEnd = this->staticAdvancedRenderQueueBegin->next;
		delete this->staticAdvancedRenderQueueBegin;
		this->staticAdvancedRenderQueueBegin = this->staticAdvancedRenderQueueEnd;
	}
	SAFE_COM_RELEASE(this->advancedIndexBuffer);
	SAFE_COM_RELEASE(this->advancedVertexBuffer);
	SAFE_COM_RELEASE(this->simpleIndexBuffer);
	SAFE_COM_RELEASE(this->simpleVertexBuffer);
	SAFE_COM_RELEASE(this->staticAdvancedIndexBuffer);
	SAFE_COM_RELEASE(this->staticAdvancedVertexBuffer);
	SAFE_COM_RELEASE(this->staticSimpleIndexBuffer);
	SAFE_COM_RELEASE(this->staticSimpleVertexBuffer);
	SAFE_COM_RELEASE(this->advancedVertexDecl);
	SAFE_COM_RELEASE(this->simpleVertexDecl);
}

//matrix work
void ISceneManager::SetProjMatrix(const D3DXMATRIXA16 &newProjMatrix)
{
	this->projectionMatrix= newProjMatrix;
	this->D3DDevice->SetTransform(D3DTS_PROJECTION, &newProjMatrix);
}

void ISceneManager::SetWorldMatrix(const D3DXMATRIXA16 &newWorldMatrix)
{
	this->worldMatrix = newWorldMatrix;
	this->D3DDevice->SetTransform(D3DTS_WORLD, &newWorldMatrix);
}

void ISceneManager::SetViewMatrix(const D3DXMATRIXA16 &newViewMatrix)
{
	this->viewMatrix = newViewMatrix;
	this->D3DDevice->SetTransform(D3DTS_VIEW, &newViewMatrix);
}

//dynamic buffers
HRESULT ISceneManager::CreateAdvancedIndexBuffer(unsigned int size)
{
	return this->D3DDevice->CreateIndexBuffer(size*sizeof(DWORD), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFMT_INDEX32,D3DPOOL_DEFAULT, &this->advancedIndexBuffer,NULL);
}

HRESULT ISceneManager::CreateAdvancedVertexBuffer(unsigned int size)
{
	return this->D3DDevice->CreateVertexBuffer(size*sizeof(AdvancedVertex), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &this->advancedVertexBuffer, NULL);
}

HRESULT ISceneManager::CreateSimpleIndexBuffer(unsigned int size)
{
	return this->D3DDevice->CreateIndexBuffer(size*sizeof(DWORD), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFMT_INDEX32,D3DPOOL_DEFAULT, &this->simpleIndexBuffer,NULL);
}

HRESULT ISceneManager::CreateSimpleVertexBuffer(unsigned int size)
{
	return this->D3DDevice->CreateVertexBuffer(size*sizeof(SimpleVertex), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &this->simpleVertexBuffer, NULL);
}

//static buffers
HRESULT ISceneManager::CreateStaticAdvancedIndexBuffer(unsigned int size)
{
	return this->D3DDevice->CreateIndexBuffer(size*sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32,D3DPOOL_DEFAULT, &this->staticAdvancedIndexBuffer,NULL);
}

HRESULT ISceneManager::CreateStaticAdvancedVertexBuffer(unsigned int size)
{
	return this->D3DDevice->CreateVertexBuffer(size*sizeof(AdvancedVertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &this->staticAdvancedVertexBuffer, NULL);
}

HRESULT ISceneManager::CreateStaticSimpleIndexBuffer(unsigned int size)
{
	return this->D3DDevice->CreateIndexBuffer(size*sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32,D3DPOOL_DEFAULT, &this->staticSimpleIndexBuffer,NULL);
}

HRESULT ISceneManager::CreateStaticSimpleVertexBuffer(unsigned int size)
{
	return this->D3DDevice->CreateVertexBuffer(size*sizeof(SimpleVertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &this->staticSimpleVertexBuffer, NULL);
}
//scene begin / end
void ISceneManager::SceneBegin()
{
	this->D3DDevice->SetTransform(D3DTS_PROJECTION, &this->projectionMatrix);
	this->D3DDevice->SetTransform(D3DTS_VIEW, &this->viewMatrix);
	this->D3DDevice->SetTransform(D3DTS_WORLD, &this->worldMatrix);
	this->currentDeviceTexture[0]  = NULL;
	this->D3DDevice->SetTexture(0,NULL);
}

void ISceneManager::SceneEnd()
{
	//static simple
	checker = false;
	this->verticesCount = 0;
	this->indicesCount = 0;
	this->verticesBaseIndex = 0;
	this->startIndex = 0;
	this->tempStaticSimpleRenderQueue = this->staticSimpleRenderQueueBegin;
	////если очередь не пуста, то ищем меш который подлежит рендеру
	if(this->tempStaticSimpleRenderQueue != NULL)
		while(!this->tempStaticSimpleRenderQueue->mesh->IsRenderable())
		{
			if(this->tempStaticSimpleRenderQueue->next != NULL)
				if(!this->tempStaticSimpleRenderQueue->next->mesh->GetDataFromPrev() && !this->tempStaticSimpleRenderQueue->mesh->GetDataFromPrev())
				{
					this->verticesBaseIndex += this->tempStaticSimpleRenderQueue->mesh->GetVerticesCount();
					this->startIndex += this->tempStaticSimpleRenderQueue->mesh->GetIndicesCount();
				}
			this->tempStaticSimpleRenderQueue = this->tempStaticSimpleRenderQueue->next;
			if(this->tempStaticSimpleRenderQueue == NULL)
				break;
		}
	//рендерим
	if(this->tempStaticSimpleRenderQueue != NULL)
	{
		this->verticesCount = this->tempStaticSimpleRenderQueue->mesh->GetVerticesCount();
		this->indicesCount = this->tempStaticSimpleRenderQueue->mesh->GetIndicesCount();
		this->currentStateBlock = this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock() == NULL ? NULL : this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock()->GetData();
		this->currentSimpleTexture = this->tempStaticSimpleRenderQueue->mesh->GetTexture();
		this->currentEffect = this->tempStaticSimpleRenderQueue->mesh->GetEffect();
		this->currentWorldMatrix = this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix() == NULL ?  NULL : this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix()->GetData();
		
		this->D3DDevice->SetIndices(this->staticSimpleIndexBuffer);
		this->D3DDevice->SetVertexDeclaration(this->simpleVertexDecl);
		this->D3DDevice->SetStreamSource(0,this->staticSimpleVertexBuffer, 0,sizeof(SimpleVertex));
		while(this->tempStaticSimpleRenderQueue != NULL)
		{
			if(this->verticesCount == 0 && !this->tempStaticSimpleRenderQueue->mesh->IsRenderable())
			{
				if(this->tempStaticSimpleRenderQueue->next != NULL)
					if(!this->tempStaticSimpleRenderQueue->next->mesh->GetDataFromPrev() && !this->tempStaticSimpleRenderQueue->mesh->GetDataFromPrev())
					{
						this->verticesBaseIndex += this->tempStaticSimpleRenderQueue->mesh->GetVerticesCount();
						this->startIndex += this->tempStaticSimpleRenderQueue->mesh->GetIndicesCount();
					}
				this->tempStaticSimpleRenderQueue = this->tempStaticSimpleRenderQueue->next;
				continue;
			}
			if(this->verticesCount == 0)
			{
				this->verticesCount = this->tempStaticSimpleRenderQueue->mesh->GetVerticesCount();
				this->indicesCount = this->tempStaticSimpleRenderQueue->mesh->GetIndicesCount();
				this->currentStateBlock = this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock() == NULL ? NULL : this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock()->GetData();
				this->currentSimpleTexture = this->tempStaticSimpleRenderQueue->mesh->GetTexture();
				this->currentEffect = this->tempStaticSimpleRenderQueue->mesh->GetEffect();
				this->currentWorldMatrix = this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix() == NULL ?  NULL : this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix()->GetData();
			}
			checker = (this->tempStaticSimpleRenderQueue->next == NULL);
			if(!checker)
				checker = !this->tempStaticSimpleRenderQueue->next->mesh->IsRenderable();
			if(checker)
			{
				if(this->currentStateBlock != NULL)
					this->currentStateBlock->Apply();
				if(this->currentDeviceTexture[0] != this->currentSimpleTexture)
				{
					this->D3DDevice->SetTexture(0,this->currentSimpleTexture);
					this->currentDeviceTexture[0] = this->currentSimpleTexture;
				}
				if(this->currentWorldMatrix  != NULL)
					this->D3DDevice->SetTransform(D3DTS_WORLD, this->currentWorldMatrix);
				else
					this->D3DDevice->SetTransform(D3DTS_WORLD, &this->worldMatrix);
				if(this->currentEffect != NULL)
				{
					this->currentEffect->Begin(&this->pass,NULL);
					for(unsigned int i = 0; i < pass; i++)
					{
						this->currentEffect->BeginPass(this->pass);
						this->D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, this->verticesBaseIndex, 0, this->verticesCount, this->startIndex, (int)indicesCount/3);
						this->currentEffect->EndPass();
					}
					this->currentEffect->End();
				}else
					this->D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, this->verticesBaseIndex, 0, this->verticesCount, this->startIndex, (int)indicesCount/3);
				if(this->tempStaticSimpleRenderQueue->next != NULL)
				{
					if(!this->tempStaticSimpleRenderQueue->mesh->GetDataFromPrev())
					{
						this->verticesBaseIndex += this->tempStaticSimpleRenderQueue->mesh->GetVerticesCount();
						this->startIndex += this->tempStaticSimpleRenderQueue->mesh->GetIndicesCount();
					}
					this->tempStaticSimpleRenderQueue = this->tempStaticSimpleRenderQueue->next;
					if(!this->tempStaticSimpleRenderQueue->mesh->IsRenderable())
					{
						this->verticesCount = 0;
						continue;
					}
					this->verticesCount = this->tempStaticSimpleRenderQueue->mesh->GetVerticesCount();
					this->indicesCount = this->tempStaticSimpleRenderQueue->mesh->GetIndicesCount();
					this->currentStateBlock = this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock() == NULL ? NULL : this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock()->GetData();
					this->currentSimpleTexture = this->tempStaticSimpleRenderQueue->mesh->GetTexture();
					this->currentEffect = this->tempStaticSimpleRenderQueue->mesh->GetEffect();
					this->currentWorldMatrix = this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix() == NULL ?  NULL : this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix()->GetData();
					continue;
				}else 
					break;
			}
			this->tempStaticSimpleRenderQueue = this->tempStaticSimpleRenderQueue->next;
			if( this->currentStateBlock == (this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock() == NULL ?  NULL : this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock()->GetData()) &&
				this->tempStaticSimpleRenderQueue->mesh->GetTexture() == this->currentSimpleTexture &&
				this->currentWorldMatrix == (this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix() == NULL ? NULL : this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix()->GetData()) && 
				this->currentEffect == this->tempStaticSimpleRenderQueue->mesh->GetEffect()
				&& this->tempStaticSimpleRenderQueue->mesh->IsRenderable() && !this->tempStaticSimpleRenderQueue->mesh->GetDataFromPrev())
			{
				this->verticesCount += this->tempStaticSimpleRenderQueue->mesh->GetVerticesCount();
				this->indicesCount += this->tempStaticSimpleRenderQueue->mesh->GetIndicesCount();
			}else{
				if(this->currentStateBlock != NULL)
					this->currentStateBlock->Apply();
				if(this->currentDeviceTexture[0] != this->currentSimpleTexture)
				{
					this->D3DDevice->SetTexture(0,this->currentSimpleTexture);
					this->currentDeviceTexture[0] = this->currentSimpleTexture;
				}
				if(this->currentWorldMatrix  != NULL)
					this->D3DDevice->SetTransform(D3DTS_WORLD, this->currentWorldMatrix);
				else
					this->D3DDevice->SetTransform(D3DTS_WORLD, &this->worldMatrix);
				if(this->currentEffect != NULL)
				{
					this->currentEffect->Begin(&this->pass,NULL);
					for(unsigned  int i = 0; i < pass; i++)
					{
						this->currentEffect->BeginPass(this->pass);
						this->D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, this->verticesBaseIndex, 0, this->verticesCount, this->startIndex, (int)indicesCount/3);
						this->currentEffect->EndPass();
					}
					this->currentEffect->End();
				}else
					this->D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, this->verticesBaseIndex, 0, this->verticesCount, this->startIndex, (int)indicesCount/3);

				if(this->tempStaticSimpleRenderQueue->mesh->GetDataFromPrev())
				{
					this->verticesCount = this->tempStaticSimpleRenderQueue->mesh->GetVerticesCount();
					this->indicesCount = this->tempStaticSimpleRenderQueue->mesh->GetIndicesCount();
					this->currentStateBlock = this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock() == NULL ? NULL : this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock()->GetData();
					this->currentSimpleTexture = this->tempStaticSimpleRenderQueue->mesh->GetTexture();
					this->currentEffect = this->tempStaticSimpleRenderQueue->mesh->GetEffect();
					this->currentWorldMatrix = this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix() == NULL ?  NULL : this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix()->GetData();
					if(this->currentStateBlock != NULL)
						this->currentStateBlock->Apply();
					if(this->currentDeviceTexture[0] != this->currentSimpleTexture)
					{
						this->D3DDevice->SetTexture(0,this->currentSimpleTexture);
						this->currentDeviceTexture[0] = this->currentSimpleTexture;
					}
					if(this->currentWorldMatrix  != NULL)
						this->D3DDevice->SetTransform(D3DTS_WORLD, this->currentWorldMatrix);
					else
						this->D3DDevice->SetTransform(D3DTS_WORLD, &this->worldMatrix);
					if(this->currentEffect != NULL)
					{
						this->currentEffect->Begin(&this->pass,NULL);
						for(unsigned  int i = 0; i < pass; i++)
						{
							this->currentEffect->BeginPass(this->pass);
							this->D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, this->verticesBaseIndex, 0, this->verticesCount, this->startIndex, (int)indicesCount/3);
							this->currentEffect->EndPass();
						}
						this->currentEffect->End();
					}else
						this->D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, this->verticesBaseIndex, 0, this->verticesCount, this->startIndex, (int)indicesCount/3);
					
					if(this->tempStaticSimpleRenderQueue->next != NULL)
						if(!this->tempStaticSimpleRenderQueue->next->mesh->GetDataFromPrev())
						{
							this->verticesBaseIndex += this->verticesCount;
							this->startIndex += this->indicesCount;
						}
					this->tempStaticSimpleRenderQueue = this->tempStaticSimpleRenderQueue->next;
					this->verticesCount = 0;
					continue;
				}
				this->verticesBaseIndex += this->verticesCount;
				this->startIndex += this->indicesCount;
				this->verticesCount = this->tempStaticSimpleRenderQueue->mesh->GetVerticesCount();
				this->indicesCount = this->tempStaticSimpleRenderQueue->mesh->GetIndicesCount();
				this->currentStateBlock = this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock() == NULL ? NULL : this->tempStaticSimpleRenderQueue->mesh->GetRenderStateBlock()->GetData();
				this->currentSimpleTexture = this->tempStaticSimpleRenderQueue->mesh->GetTexture();
				this->currentEffect = this->tempStaticSimpleRenderQueue->mesh->GetEffect();
				this->currentWorldMatrix = this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix() == NULL ?  NULL : this->tempStaticSimpleRenderQueue->mesh->GetWorldMatrix()->GetData();
			}
		}
	}


	//DYNAMIC
	/*if(this->advancedTrianglesCount > 0 )
	{
		this->advancedVertexBuffer->Unlock();
		this->advancedIndexBuffer->Unlock();
		this->D3DDevice->SetIndices(this->advancedIndexBuffer);
		this->D3DDevice->SetVertexDeclaration(this->advancedVertexDecl);
		this->D3DDevice->SetStreamSource(0,this->advancedVertexBuffer, 0,sizeof(AdvancedVertex));
		if(this->currentDeviceTexture[0] != this->currentAdvancedTexture)
		{
			this->D3DDevice->SetTexture(0,this->currentAdvancedTexture);
			this->currentDeviceTexture[0] = this->currentAdvancedTexture;
		}
		this->D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,this->advancedTrianglesCount*3,0,this->advancedTrianglesCount);
		this->advancedTrianglesCount = 0;
	}

	if(this->simpleTrianglesCount > 0)
	{
		this->simpleVertexBuffer->Unlock();
		this->simpleIndexBuffer->Unlock();
		this->D3DDevice->SetIndices(this->simpleIndexBuffer);
		this->D3DDevice->SetVertexDeclaration(this->simpleVertexDecl);
		this->D3DDevice->SetStreamSource(0,this->simpleVertexBuffer, 0,sizeof(SimpleVertex));
		if(this->currentDeviceTexture[0] != this->currentSimpleTexture)
		{
			this->D3DDevice->SetTexture(0,this->currentSimpleTexture);
			this->currentDeviceTexture[0] = this->currentSimpleTexture;
		}
		this->D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,this->simpleTrianglesCount*3,0,this->simpleTrianglesCount);
		this->simpleTrianglesCount = 0;
	}*/
	//static
}

void ISceneManager::StartSimpleRenderQueueFill()
{
	this->staticSimpleVertexBuffer->Lock(0,0,(void**)&this->staticSimpleVertices,NULL);
	this->staticSimpleIndexBuffer->Lock(0,0,(void**)&this->staticSimpleIndices,NULL);
}

void ISceneManager::EndSimpleRenderQueueFill()
{
	this->staticSimpleVertexBuffer->Unlock();
	this->staticSimpleIndexBuffer->Unlock();
}

void ISceneManager::StartAdvancedRenderQueueFill()
{
	this->staticAdvancedVertexBuffer->Lock(0,0,(void**)this->staticAdvancedVertices,NULL);
	this->staticAdvancedIndexBuffer->Lock(0,0,(void**)this->staticAdvancedIndices,NULL);
}

void ISceneManager::EndAdvancedRenderQueueFill()
{
	this->staticAdvancedVertexBuffer->Unlock();
	this->staticAdvancedIndexBuffer->Unlock();
}

void ISceneManager::AddStaticSimpleMesh(ISimpleMesh *mesh)
{
	if(!mesh->GetDataFromPrev())
	{
		memcpy(this->staticSimpleVertices+this->totalStaticSimpleVerticesCount,mesh->GetVertices()->GetData(),mesh->GetVerticesCount()*sizeof(SimpleVertex));
		memcpy(this->staticSimpleIndices+this->totalStaticSimpleIndicesCount,mesh->GetIndices()->GetData(),mesh->GetIndicesCount()*sizeof(DWORD));

		this->totalStaticSimpleVerticesCount += mesh->GetVerticesCount();
		this->totalStaticSimpleIndicesCount += mesh->GetIndicesCount();
	}

	StaticSimpleRenderQueue *temp = new StaticSimpleRenderQueue(mesh);
	if(this->staticSimpleRenderQueueBegin == NULL)
	{
		this->staticSimpleRenderQueueBegin = this->staticSimpleRenderQueueEnd =  temp;
	}else{
		this->staticSimpleRenderQueueEnd->next = temp;
		this->staticSimpleRenderQueueEnd = temp;
	}
}

void ISceneManager::ClearSimpleRenderQueue()
{
	this->totalStaticSimpleVerticesCount = 0;
	this->totalStaticSimpleIndicesCount = 0;
	while(this->staticSimpleRenderQueueBegin != NULL)
	{
		this->staticSimpleRenderQueueEnd = this->staticSimpleRenderQueueBegin->next;
		delete this->staticSimpleRenderQueueBegin;
		this->staticSimpleRenderQueueBegin = this->staticSimpleRenderQueueEnd;
	}
}

void ISceneManager::AddStaticAdvancedMesh(IMesh *mesh)
{
	memcpy(this->staticAdvancedVertices+this->totalStaticAdvancedVerticesCount,mesh->GetVertices(),mesh->GetVerticesCount()*sizeof(AdvancedVertex));
	memcpy(this->staticAdvancedIndices+this->totalStaticAdvancedIndicesCount,mesh->GetIndices(),mesh->GetIndicesCount()*sizeof(DWORD));

	this->totalStaticAdvancedVerticesCount += mesh->GetVerticesCount();
	this->totalStaticAdvancedIndicesCount += mesh->GetIndicesCount();

	StaticAdvancedRenderQueue *temp = new StaticAdvancedRenderQueue(mesh);
	if(this->staticAdvancedRenderQueueBegin == NULL)
	{
		this->staticAdvancedRenderQueueBegin = this->staticAdvancedRenderQueueEnd =  temp;
	}else{
		this->staticAdvancedRenderQueueEnd->next = temp;
	}
}

void ISceneManager::ClearAdvancedRenderQueue()
{
	this->totalStaticAdvancedVerticesCount = 0;
	this->totalStaticAdvancedIndicesCount = 0;

	while(this->staticAdvancedRenderQueueBegin != NULL)
	{
		this->staticAdvancedRenderQueueEnd = this->staticAdvancedRenderQueueBegin->next;
		delete this->staticAdvancedRenderQueueBegin;
		this->staticAdvancedRenderQueueBegin = this->staticAdvancedRenderQueueEnd;
	}
}


void ISceneManager::ReleaseAllBuffers()
{
	SAFE_COM_RELEASE(this->advancedIndexBuffer);
	SAFE_COM_RELEASE(this->advancedVertexBuffer);
	SAFE_COM_RELEASE(this->simpleIndexBuffer);
	SAFE_COM_RELEASE(this->simpleVertexBuffer);
	SAFE_COM_RELEASE(this->staticAdvancedIndexBuffer);
	SAFE_COM_RELEASE(this->staticAdvancedVertexBuffer);
	SAFE_COM_RELEASE(this->staticSimpleIndexBuffer);
	SAFE_COM_RELEASE(this->staticSimpleVertexBuffer);
}