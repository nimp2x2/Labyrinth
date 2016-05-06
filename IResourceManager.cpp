#include "IResourceManager.h"

IResourceManager::IResourceManager(LPDIRECT3DDEVICE9 device)
{
	this->device = device;
	this->firstTexture = NULL;
	this->lastTexture = NULL;
	this->firstEffect = NULL;
	this->lastEffect = NULL;
	this->firstSimpleMesh = NULL;
	this->lastSimpleMesh = NULL;
}

IResourceManager::~IResourceManager()
{
	this->PurgeAll();
}

bool IResourceManager::LoadTexture(wchar_t* texturePath)
{
	LPDIRECT3DTEXTURE9 tempTex;
	if(FAILED(D3DXCreateTextureFromFile(this->device,texturePath,&tempTex)))
		return false;
	if(this->firstTexture == NULL)
	{
		this->firstTexture = new resourceQueue<IDirect3DTexture9>;
		this->firstTexture->resourcePath = texturePath;
		this->firstTexture->resource = tempTex;
		this->firstTexture->next = NULL;
		this->lastTexture = this->firstTexture;
	}else{
		resourceQueue<IDirect3DTexture9> *temp = new resourceQueue<IDirect3DTexture9>;
		temp->resourcePath = texturePath;
		temp->resource = tempTex;
		temp->next = NULL;
		this->lastTexture->next = temp;
		this->lastTexture = temp;
	}
	return true;
}

LPDIRECT3DTEXTURE9 IResourceManager::GetTexture(wchar_t* texturePath)
{
	resourceQueue<IDirect3DTexture9> *temp;
	temp = this->firstTexture;

	while(temp != NULL)
	{
		if(wcscmp(temp->resourcePath,texturePath) == 0)
			return temp->resource;
		else
			temp = temp->next;
	}
	return NULL;
}

bool IResourceManager::UnloadTexture(wchar_t* texturePath)
{
	if(this->firstTexture != NULL)
	{
		resourceQueue<IDirect3DTexture9> *temp,*temp2; 
		if(wcscmp(this->firstTexture->resourcePath,texturePath) == 0)
		{
			this->firstTexture->resource->Release();
			temp = this->firstTexture->next;
			delete this->firstTexture;
			this->firstTexture = temp;
			if(this->firstTexture == NULL)
			{
				this->lastTexture = NULL;
			}
			return true;
		}
		
		temp = this->firstTexture;
		while(temp->next != NULL)
		{
			if(wcscmp(temp->next->resourcePath,texturePath) == 0)
			{
				if(temp->next == this->lastTexture)
					this->lastTexture = temp;
				temp->next->resource->Release();
				temp2 = temp->next->next;
				delete temp->next;
				temp->next = temp2;
				return true;
			}else
				temp = temp->next;
		}
	}
	return false;
}

void IResourceManager::PurgeTexQueue()
{
	resourceQueue<IDirect3DTexture9> *temp;
	while(this->firstTexture != NULL)
	{
		temp = this->firstTexture->next;
		this->firstTexture->resource->Release();
		delete this->firstTexture;
		this->firstTexture = temp;
	}
	this->lastTexture= NULL;
}

void IResourceManager::PurgeAll()
{
	this->PurgeTexQueue();
	this->PurgeEffectQueue();
	this->PurgeSimpleMeshQueue();
}

void IResourceManager::PurgeEffectQueue()
{
	resourceQueue<ID3DXEffect> *temp;
	while(this->firstEffect != NULL)
	{
		temp = this->firstEffect->next;
		this->firstEffect->resource->Release();
		delete this->firstEffect;
		this->firstEffect = temp;
	}
	this->lastEffect= NULL;
}

bool IResourceManager::UnloadEffect(wchar_t *effectName)
{
	if(this->firstEffect != NULL)
	{
		resourceQueue<ID3DXEffect> *temp,*temp2; 
		if(wcscmp(this->firstEffect->resourcePath,effectName) == 0)
		{
			this->firstEffect->resource->Release();
			temp = this->firstEffect->next;
			delete this->firstEffect;
			this->firstEffect = temp;
			if(this->firstEffect == NULL)
			{
				this->lastEffect = NULL;
			}
			return true;
		}
		
		temp = this->firstEffect;
		while(temp->next != NULL)
		{
			if(wcscmp(temp->next->resourcePath,effectName) == 0)
			{
				if(temp->next == this->lastEffect)
					this->lastEffect = temp;
				temp->next->resource->Release();
				temp2 = temp->next->next;
				delete temp->next;
				temp->next = temp2;
				return true;
			}else
				temp = temp->next;
		}
	}
	return false;
}

ID3DXEffect* IResourceManager::GetEffect(wchar_t* effectName)
{
	resourceQueue<ID3DXEffect> *temp;
	temp = this->firstEffect;

	while(temp != NULL)
	{
		if(wcscmp(temp->resourcePath,effectName) == 0)
			return temp->resource;
		else
			temp = temp->next;
	}
	return NULL;
}

bool IResourceManager::LoadEffect(wchar_t* effectName)
{
	ID3DXEffect *tempEffect;
	if(FAILED(D3DXCreateEffectFromFile(this->device,effectName,NULL,NULL,NULL,NULL,&tempEffect,NULL)))
		return false;
	
	if(this->firstEffect == NULL)
	{
		this->firstEffect = new resourceQueue<ID3DXEffect>;
		this->firstEffect->resourcePath = effectName;
		this->firstEffect->resource = tempEffect;
		this->firstEffect->next = NULL;
		this->lastEffect = this->firstEffect;
	}else{
		resourceQueue<ID3DXEffect> *temp = new resourceQueue<ID3DXEffect>;
		temp->resourcePath = effectName;
		temp->resource = tempEffect;
		temp->next = NULL;
		this->lastEffect->next = temp;
		this->lastEffect = temp;
	}
	return true;
}

bool IResourceManager::LoadSimpleMesh(wchar_t* meshName, LPDIRECT3DTEXTURE9 meshTexture)
{
	ISimpleMesh *tempMesh;
	FILE *file;
	_wfopen_s(&file,meshName,L"rb");
	if(file == NULL)
		return false;
	DWORD verticesCount;
	DWORD additionalVertices;
	DWORD indicesCount;
	DWORD textureVertices;
	bool founded = false;
	fread(&verticesCount,sizeof(DWORD),1,file);
	fread(&additionalVertices,sizeof(DWORD),1,file);
	fread(&indicesCount,sizeof(DWORD),1,file);
	fread(&textureVertices,sizeof(DWORD),1,file);
	SimpleVertex *vertices = new SimpleVertex[verticesCount+additionalVertices];
	additionalVertices = 0;
	//position reading
	for(DWORD i = 0; i < verticesCount; i++)
	{
		fread(&vertices[i].position[0],sizeof(float),1,file);
		fread(&vertices[i].position[1],sizeof(float),1,file);
		fread(&vertices[i].position[2],sizeof(float),1,file);
	}
	//indices reading
	DWORD *indices = new DWORD[indicesCount*3];
	for(DWORD i = 0; i < indicesCount*3; i++)
	{
		fread(&indices[i],sizeof(DWORD),1,file);
		vertices[indices[i]].texCoord[0] = -100.0f;
	}
	//texture vertices reading
	float *texture = new float[textureVertices*3];
	for(DWORD i = 0; i < textureVertices*3; i++)
	{
		fread(&texture[i],sizeof(float),1,file);
	}
	DWORD textureInd;
	for(DWORD i = 0; i < indicesCount*3; i++)
	{
		fread(&textureInd,sizeof(DWORD),1,file);
		if(vertices[indices[i]].texCoord[0] == -100.0f)
		{
			vertices[indices[i]].texCoord[0] = texture[textureInd*3];
			vertices[indices[i]].texCoord[1] = 1.0f-texture[textureInd*3+1];
		}else{
			founded = false;
			for(DWORD j = verticesCount-1; j < verticesCount+additionalVertices; j++)
				if(vertices[j].position[0] == vertices[indices[i]].position[0])
					if(vertices[j].position[1] == vertices[indices[i]].position[1])
						if(vertices[j].position[2] == vertices[indices[i]].position[2])
								if(vertices[j].texCoord[0] == texture[textureInd*3])
										if(vertices[j].texCoord[1] == 1.0f-texture[textureInd*3+1])
										{
											indices[i] = j;
											founded = true;
											break;
										}
			if(!founded)
			{
				vertices[verticesCount+additionalVertices].position[0] = vertices[indices[i]].position[0];
				vertices[verticesCount+additionalVertices].position[1] = vertices[indices[i]].position[1];
				vertices[verticesCount+additionalVertices].position[2] = vertices[indices[i]].position[2];
				vertices[verticesCount+additionalVertices].texCoord[0] = texture[textureInd*3];
				vertices[verticesCount+additionalVertices].texCoord[1] = 1.0f-texture[textureInd*3+1];
				indices[i] = verticesCount + additionalVertices;
				additionalVertices++;
			}
		}
	}
	tempMesh = new ISimpleMesh(vertices,verticesCount+additionalVertices,indices,indicesCount*3,meshTexture,NULL,NULL,NULL);
	delete []texture;
	if(this->firstSimpleMesh == NULL)
	{
		this->firstSimpleMesh = new resourceQueue<ISimpleMesh>;
		this->firstSimpleMesh->resourcePath = meshName;
		this->firstSimpleMesh->resource = tempMesh;
		this->firstSimpleMesh->next = NULL;
		this->lastSimpleMesh = this->firstSimpleMesh;
	}else{
		resourceQueue<ISimpleMesh> *temp = new resourceQueue<ISimpleMesh>;
		temp->resourcePath = meshName;
		temp->resource = tempMesh;
		temp->next = NULL;
		this->lastSimpleMesh->next = temp;
		this->lastSimpleMesh = temp;
	}
	return true;
}

bool IResourceManager::UnloadSimpleMesh(wchar_t* meshName)
{
	if(this->firstSimpleMesh != NULL)
	{
		resourceQueue<ISimpleMesh> *temp,*temp2; 
		if(wcscmp(this->firstSimpleMesh->resourcePath,meshName) == 0)
		{
			delete this->firstSimpleMesh->resource;
			temp = this->firstSimpleMesh->next;
			delete this->firstSimpleMesh;
			this->firstSimpleMesh = temp;
			if(this->firstSimpleMesh == NULL)
			{
				this->lastSimpleMesh= NULL;
			}
			return true;
		}
		
		temp = this->firstSimpleMesh;
		while(temp->next != NULL)
		{
			if(wcscmp(temp->next->resourcePath,meshName) == 0)
			{
				if(temp->next == this->lastSimpleMesh)
					this->lastSimpleMesh = temp;
				delete temp->next->resource;
				temp2 = temp->next->next;
				delete temp->next;
				temp->next = temp2;
				return true;
			}else
				temp = temp->next;
		}
	}
	return false;
}

ISimpleMesh* IResourceManager::GetSimpleMesh(wchar_t* meshName)
{
	resourceQueue<ISimpleMesh> *temp;
	temp = this->firstSimpleMesh;

	while(temp != NULL)
	{
		if(wcscmp(temp->resourcePath,meshName) == 0)
			return temp->resource;
		else
			temp = temp->next;
	}
	return NULL;
}

void IResourceManager::PurgeSimpleMeshQueue()
{
	resourceQueue<ISimpleMesh> *temp;
	while(this->firstSimpleMesh != NULL)
	{
		temp = this->firstSimpleMesh->next;
		delete this->firstSimpleMesh->resource;
		delete this->firstSimpleMesh;
		this->firstSimpleMesh = temp;
	}
	this->lastSimpleMesh = NULL;
}

bool IResourceManager::LoadSimpleMesh(wchar_t* meshName, ISimpleMesh *mesh)
{
	if(this->firstSimpleMesh == NULL)
	{
		this->firstSimpleMesh = new resourceQueue<ISimpleMesh>;
		this->firstSimpleMesh->resourcePath = meshName;
		this->firstSimpleMesh->resource = mesh;
		this->firstSimpleMesh->next = NULL;
		this->lastSimpleMesh = this->firstSimpleMesh;
	}else{
		resourceQueue<ISimpleMesh> *temp = new resourceQueue<ISimpleMesh>;
		temp->resourcePath = meshName;
		temp->resource = mesh;
		temp->next = NULL;
		this->lastSimpleMesh->next = temp;
		this->lastSimpleMesh = temp;
	}
	return true;
}